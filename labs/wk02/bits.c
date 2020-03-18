// COMP1521 19t2 ... lab 2 warm-up

#include <stdio.h>
#include <stdlib.h>

static void range_uchar (void);
static void range_char (void);
static void range_uhint (void);
static void range_hint (void);
static void range_int (void);
static void range_uint (void);
static void range_lint (void);
static void range_ulint (void);
static void range_llint (void);
static void range_ullint (void);

int main (void)
{
	range_char ();
	range_uchar ();
	range_hint ();
	range_uhint ();
	range_int ();
	range_uint ();
	range_lint ();
	range_ulint ();
	range_llint ();
	range_ullint ();

	return EXIT_SUCCESS;
}

// Calculate and print the largest and smallest `int` values
static void range_int (void)
{
    int i = 1;
	int int_min = (i << (sizeof(int)*8 - 1));
	int int_max = ~(i << (sizeof(int)*8 - 1));
	
	printf (
		"                   int (%zu bytes): %d ... %d\n",
		sizeof (int), int_min, int_max
	);
}

// Calculate and print the largest and smallest `unsigned int` values
static void range_uint (void)
{
    unsigned i = 0;
	unsigned int uint_min =  (i << (sizeof(unsigned int)*8 - 1));
	unsigned int uint_max = ~(i << (sizeof(unsigned int)*8 - 1));
	printf (
		"          unsigned int (%zu bytes): %u ... %u\n",
		sizeof (unsigned int), uint_min, uint_max
	);
}

// Calculate and print the largest and smallest `long int` values
static void range_lint (void)
{
    long int i = 1;
	long int long_min =  (i << (sizeof(long int)*8 - 1));
	long int long_max = ~(i << (sizeof(long int)*8 - 1));
	printf (
		"              long int (%zu bytes): %ld ... %ld\n",
		sizeof (long int), long_min, long_max
	);
}

// Calculate and print the largest and smallest `unsigned long int` values
static void range_ulint (void)
{
    unsigned long int i = 0;
	unsigned long int ulong_min =  (i << (sizeof(unsigned long int)*8 - 1));
	unsigned long int ulong_max = ~(i << (sizeof(unsigned long int)*8 - 1));
	printf (
		"     unsigned long int (%zu bytes): %lu ... %lu\n",
		sizeof (unsigned long int), ulong_min, ulong_max
	);
}

// Calculate and print the largest and smallest `long long int` values
static void range_llint (void)
{
	long long int i = 1;
	long long int llong_min =  (i << (sizeof(long long int)*8 - 1));
	long long int llong_max = ~(i << (sizeof(long long int)*8 - 1));
	printf (
		"         long long int (%zu bytes): %lld ... %lld\n",
		sizeof (long long int), llong_min, llong_max
	);
}

// Calculate and print the largest and smallest `unsigned long long int` values
static void range_ullint (void)
{
    unsigned long long int i = 0;
	unsigned long long int ullong_min =  (i << (sizeof(unsigned long long int)*8 - 1));
	unsigned long long int ullong_max = ~(i << (sizeof(unsigned long long int)*8 - 1));
	printf (
		"unsigned long long int (%zu bytes): %llu ... %llu\n",
		sizeof (unsigned long long int), ullong_min, ullong_max
	);
}

// Calculate and print the largest and smallest `short int` values
static void range_hint (void)
{
    short int i = 1;
	short int hint_min =  (i << (sizeof(short int)*8 - 1));
	short int hint_max = ~(i << (sizeof(short int)*8 - 1));
	printf (
		"             short int (%zu bytes): %hd ... %hd\n",
		sizeof (short int), hint_min, hint_max
	);
}

// Calculate and print the largest and smallest `unsigned short int` values
static void range_uhint (void)
{
    unsigned short int i = 0;
	unsigned short int uhint_min =  (i << (sizeof(unsigned short int)*8 - 1));
	unsigned short int uhint_max = ~(i << (sizeof(unsigned short int)*8 - 1));
	printf (
		"    unsigned short int (%zu bytes): %hu ... %hu\n",
		sizeof (unsigned short int), uhint_min, uhint_max
	);
}

// Calculate and print the largest and smallest `char` values
static void range_char (void)
{
    char i = 1;
	char char_min =  (i << (sizeof(char)*8 - 1));
	char char_max = ~(i << (sizeof(char)*8 - 1));
	printf (
		"                  char (%zu bytes): %d ... %d\n",
		sizeof (char), char_min, char_max
	);
}

// Calculate and print the largest and smallest `unsigned char` values
static void range_uchar (void)
{
    unsigned char i = 0;
	unsigned char uchar_min =  (i << (sizeof(unsigned char)*8 - 1));
	unsigned char uchar_max = ~(i << (sizeof(unsigned char)*8 - 1));
	printf (
		"         unsigned char (%zu bytes): %u ... %u\n",
		sizeof (unsigned char), uchar_min, uchar_max
	);
}

