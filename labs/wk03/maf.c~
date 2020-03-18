// COMP1521 19t2 ... lab 03: Make a Float
// maf.c: read in bit strings to build a float value
// Written by John Shepherd, August 2017
// Completed by ...

#include <assert.h>
#include <err.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

typedef uint32_t word;

typedef struct float32 {
	// define bit_fields for sign, exp and frac
	// obviously they need to be larger than 1-bit each
	// and may need to be defined in a different order
	unsigned int frac : 23, exp : 8, sign : 1;
} float32;

typedef union bits32 {
	float fval;   // interpret the bits as a float
	word xval;    // interpret as a single 32-bit word
	float32 bits; // manipulate individual bits
} bits32;

void checkArgs (int, char **);
bits32 getBits (char *, char *, char *);
char *showBits (word, char *);
bool justBits (char *, int);
unsigned int converter (char *, int);
unsigned int power(int, int);

int main (int argc, char **argv)
{
	bits32 u;
	char out[35] = {'0'};

	// here's a hint ...
	u.bits.sign = u.bits.exp = u.bits.frac = 0;

	// check command-line args (all strings of 0/1
	// kills program if args are bad
	checkArgs (argc, argv);

	// convert command-line args into components of
	// a float32 inside a bits32, and return the union
	u = getBits (argv[1], argv[2], argv[3]);

	printf ("bits : %s\n", showBits (u.xval, out));
	printf ("float: %0.10f\n", u.fval);

	return EXIT_SUCCESS;
}

// convert three bit-strings (already checked)
// into the components of a struct _float
bits32 getBits (char *sign, char *exp, char *frac)
{
	bits32 new;

	// this line is just to keep gcc happy
	// delete it when you have implemented the function
	//new.bits.sign = new.bits.exp = new.bits.frac = 0;

	// convert char *sign into a single bit in new.bits
    unsigned int signint = converter(sign, 1);
    new.bits.sign = signint;
	// convert char *exp into an 8-bit value in new.bits
	unsigned int expint = converter(exp, 8);
    new.bits.exp = expint;
	// convert char *frac into a 23-bit value in new.bits
	unsigned int fracint = converter(frac, 23);
    new.bits.frac = fracint;
	return new;
}

// convert the 32-bit bit-string in val into
// a sequence of '0' and '1' characters in buf
// assume that buf has size > 32
// return a pointer to buf
char *showBits (word val, char *buf)
{
	// this line is just to keep gcc happy
	// delete it when you have implemented the function
	
    int j = 0;
    for (int i = 32; i > 0; i--) {
        if (i == 31 || i == 23) {
	        buf[j] = ' ';
	        j++;
        }
	    if (val >> (i-1) & 1) {
	        buf[j] = '1';
	        j++;
        } else {
            buf[j] = '0';
            j++;
        }
    }
	return buf;
}

// checks command-line args
// need at least 3, and all must be strings of 0/1
// never returns if it finds a problem
void checkArgs (int argc, char **argv)
{
	if (argc < 3)
		errx (EX_USAGE, "usage: %s Sign Exp Frac", argv[0]);

	if (! justBits (argv[1], 1))
		errx (EX_DATAERR, "invalid Sign: %s", argv[1]);
	if (! justBits (argv[2], 8))
		errx (EX_DATAERR, "invalid Exp: %s",  argv[2]);
	if (! justBits (argv[3], 23))
		errx (EX_DATAERR, "invalid Frac: %s", argv[3]);

	return;
}

// check whether a string is all 0/1 and of a given length
bool justBits (char *str, int len)
{
	assert (len >= 0);
	if (strlen (str) != (size_t) len)
		return false;

	while (*str != '\0') {
		if (*str != '0' && *str != '1')
			return false;
		str++;
	}
	return true;
}

unsigned int converter (char *s, int len) {
    int j = 0;
    int i = len-1;
    unsigned int total = 0;
	while (i >= 0) {
	    if (s[i] == '1')
	         total += power(2,j);
        j++;
        i--;
    }
    //printf("%s = %u\n", s, total);
    return total;
}

unsigned int power (int base, int exp) {
    unsigned int total = 1;
    for (int i = 0; i < exp; i++) {
        total *= base;
    }
    return total;
}
