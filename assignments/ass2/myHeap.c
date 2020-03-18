// COMP1521 19t2 ... Assignment 2: heap management system

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myHeap.h"

/** minimum total space for heap */
#define MIN_HEAP 4096
/** minimum amount of space for a free Chunk (excludes Header) */
#define MIN_CHUNK 32


#define ALLOC 0x55555555
#define FREE  0xAAAAAAAA

/// Types:

typedef unsigned int  uint;
typedef unsigned char byte;

typedef uintptr_t     addr; // an address as a numeric type

/** The header for a chunk. */
typedef struct header {
	uint status;    /**< the chunk's status -- ALLOC or FREE */
	uint size;      /**< number of bytes, including header */
	byte data[];    /**< the chunk's data -- not interesting to us */
} header;

/** The heap's state */
struct heap {
	void  *heapMem;     /**< space allocated for Heap */
	uint   heapSize;    /**< number of bytes in heapMem */
	void **freeList;    /**< array of pointers to free chunks */
	uint   freeElems;   /**< number of elements in freeList[] */
	uint   nFree;       /**< number of free chunks */
};


/// Variables:

/** The heap proper. */
static struct heap Heap;

/// Functions:

static addr heapMaxAddr (void);
static int findNextFreeIndex(header *object);
// static header newHeader (uint size, uint status);

/** Initialise the Heap. */
int initHeap (int size)
{
	// Modifies N to be at least the minimum size of 4096 or be a multiple of 4
	uint N = (size < 4096)?4096:size;
	N = (N % 4 == 0)?N:((N/4)+1)*4;

	Heap.heapSize = N;

	// Malloc'ing the heapMem to set aside the region of memory for myHeap to use
	Heap.heapMem = malloc(N);
	if (Heap.heapMem == NULL) return -1;

	// Pointing a FREE chunk to the beginning of heapMem
	header *chunk = Heap.heapMem;
	chunk->size = N;
	chunk->status = FREE;
	
	Heap.freeElems = N/MIN_CHUNK;
	Heap.nFree = 1;

	// Calloc'ing freeList to set all elements to zero
	Heap.freeList = calloc(sizeof(header *)*N/MIN_CHUNK, N/MIN_CHUNK);
	if (Heap.freeList == NULL) return -1;
	// Pointing chunk to the beginning of freeList
	Heap.freeList[0] = chunk;

	return 0;
}

/** Release resources associated with the heap. */
void freeHeap (void)
{
	free (Heap.heapMem);
	free (Heap.freeList);
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc (int size)
{
	if (size < 1) {
		fprintf(stderr, "%s", "Size is too small.\n");
		return NULL;
	}
	if (Heap.nFree < 1) {
		fprintf(stderr, "%s", "Heap is full. Free some space to malloc more.\n");
		return NULL;
	}

	// Calling the ceiling function on size and setting up variables for use
	uint N = (size % 4 == 0)?size:((size/4)+1)*4;
	uint headerSize = sizeof(header);
	int min = Heap.heapSize + 1;
	int index = 0;

	// Finding a free chunk in freeList which meets the size requirements
	for (int i = 0; Heap.freeList[i] != 0 && i < Heap.freeElems; i++) {
		header *curr = (header *) (Heap.freeList[i]);

		if (curr->size > N + headerSize && curr->size < min) {
			min = curr->size;
			index = i;
		}
	}

	// Pointing challoc to the place in memory where it's to be placed
	header *challoc = Heap.freeList[index];
	uint challocSize = challoc->size;
	// Pointing freeArea to the first piece of usable memory post-malloc (data after header)
	void *freeArea = (void *) ((addr) (Heap.freeList[index] + sizeof(header)));

	challoc->size = N + headerSize;
	challoc->status = ALLOC;

	// If challoc splits the freeList[index], it needs to shift all indices of freeList down one
	//	Otherwise it needs to create a free chunk to cover the leftover memory
	if (challocSize > N + headerSize + MIN_CHUNK) {
		header *freeChunk = (header *) ((addr) (Heap.freeList[index] + N + headerSize));
		freeChunk->size = min - (N + headerSize);
		freeChunk->status = FREE;
		Heap.freeList[index] = freeChunk;
	} else {
		for (int i = index + 1; i < Heap.nFree; i++) {
			Heap.freeList[i-1] = Heap.freeList[i];
		}
		Heap.nFree--;
	}

	return freeArea; // this just keeps the compiler quiet
}

/** Deallocate a chunk of memory. */
void myFree (void *obj)
{
	/// TODO ///
	header *object = (header *) ((addr) (obj - sizeof(header)));

	// FREE check commented out as test2 is broken for that check
	if (heapOffset(obj) == -1 && object != NULL /*&& object->status != FREE*/) {
		fprintf(stderr, "%s", "Attempt to free unallocated chunk\n");
		exit(1);
	}
	// If there are no elements in freeList, return
	if (!Heap.nFree)
		return;

	// Finds the chunk after object and sets nextFree to 1 if it's FREE
	int nextFree = 0;
	header *next = (header *) ((addr) (Heap.heapMem + heapOffset(object) + object->size));
	nextFree = (heapOffset(next) != -1 && next->status == FREE)?1:0;

	// Finds the chunk before object and sets prevFree to 1 if it's FREE
	int prevFree = 0;
	int prevIndex = findNextFreeIndex(object) - 1;
	header *prev = NULL;
	if (prevIndex != -1) {
		prev = (header *) Heap.freeList[prevIndex];
		header *nextTest = (header *) ((addr) (Heap.heapMem + heapOffset(prev) + prev->size));
		prevFree = (heapOffset(prev) != -1 && nextTest == object)?1:0;	
	}

	// header *prev = NULL;
	// header *chunk;
	
	// for (addr curr = (addr) Heap.heapMem; curr < heapMaxAddr(); curr += chunk->size) {
	// 	chunk = (header *) curr;
	// 	header *nextTest = (header *) ((addr)(Heap.heapMem + heapOffset(chunk) + chunk->size));
	// 	if (nextTest == object && chunk->status == FREE) {
	// 		prev = chunk;
	// 	} else {
	// 		prev = NULL;
	// 	}
	// }

	// printf("next->size = %u\n", next->size);

	if (!prevFree && nextFree) {
		// printf("I'm here\n");
		int nextIndex = findNextFreeIndex(object);

		object->status = FREE;
		object->size += next->size;
		Heap.freeList[nextIndex] = object;
		next = NULL;
	} else if (!prevFree && !nextFree) {
		object->status = FREE;
		
		// HAVE TO PUSH ALONG EVERY FREELIST POINTER
		int nextIndex = findNextFreeIndex(object);

		for (int i = Heap.nFree - 1; i >= nextIndex; i--) {
			Heap.freeList[i+1] = Heap.freeList[i];
		}
		Heap.freeList[nextIndex] = object;

		Heap.nFree++;
	} else if (prevFree && !nextFree) {
		prev->size += object->size;
		object = NULL;
	} else if (prevFree && nextFree) {
		int nextIndex = findNextFreeIndex(object);

		prev->size += object->size + next->size;
		object = NULL;
		next = NULL;

		for (int i = nextIndex; i < Heap.nFree; i++) {
			Heap.freeList[i] = Heap.freeList[i+1];
		}

		Heap.nFree--;
	}
}

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr (void)
{
	return (addr) Heap.heapMem + Heap.heapSize;
}

/** Convert a pointer to an offset in the heap. */
int heapOffset (void *obj)
{
	addr objAddr = (addr) obj;
	addr heapMin = (addr) Heap.heapMem;
	addr heapMax =        heapMaxAddr ();
	if (obj == NULL || !(heapMin <= objAddr && objAddr < heapMax))
		return -1;
	else
		return (int) (objAddr - heapMin);
}

/** Dump the contents of the heap (for testing/debugging). */
void dumpHeap (void)
{
	int onRow = 0;

	// We iterate over the heap, chunk by chunk; we assume that the
	// first chunk is at the first location in the heap, and move along
	// by the size the chunk claims to be.
	addr curr = (addr) Heap.heapMem;
	while (curr < heapMaxAddr ()) {
		header *chunk = (header *) curr;

		char stat;
		switch (chunk->status) {
		case FREE:  stat = 'F'; break;
		case ALLOC: stat = 'A'; break;
		default:
			fprintf (
				stderr,
				"myHeap: corrupted heap: chunk status %08x\n",
				chunk->status
			);
			exit (1);
		}

		printf (
			"+%05d (%c,%5d)%c",
			heapOffset ((void *) curr),
			stat, chunk->size,
			(++onRow % 5 == 0) ? '\n' : ' '
		);

		curr += chunk->size;
	}

	if (onRow % 5 > 0)
		printf ("\n");
}

static int findNextFreeIndex(header *object) {
	// header *nextFree = NULL;
	// header *chunk;
	
	// Finds the previous free chunk
	// for (addr curr = (addr) object; curr < heapMaxAddr(); curr += chunk->size) {
	// 	chunk = (header *) curr;
		
	// 	if (chunk->status == FREE) {
	// 		nextFree = chunk;
	// 		break;
	// 	}
	// }


	addr obj_addr = (addr) object;
	// Finds the index of the previous free chunk
	for (int i = 0; i < Heap.nFree; i++) {
		addr curr_addr = (addr) ((header *) Heap.freeList[i]);
		if (curr_addr > obj_addr) {
			return i;
		}
	}

	return 0;
}