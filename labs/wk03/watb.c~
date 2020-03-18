// COMP1521 19t2 ... lab 03: where are the bits?
// watb.c: determine bit-field order

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
	unsigned int a : 4;
	unsigned int b : 8;
	unsigned int c : 20;
};

int main (void)
{
	struct _bit_fields x = {0};
    x.a = 1;
    x.b = 3;
    x.c = 6;
    
	printf ("%zu\n", sizeof (x));
	unsigned int *meow = (unsigned int *) &x;
	unsigned int nek = *meow;
	
	int i = 32;
	while (i > 0) {
	    if (nek >> (i-1) & 1)
	        printf("1");
        else
            printf("0");
        i--;
    }

    printf("\n");
	return EXIT_SUCCESS;
}
