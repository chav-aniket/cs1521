// BigNum.c ... LARGE positive integer values

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum (BigNum *bn, int Nbytes)
{
	// TODO
	bn->bytes = malloc(sizeof(Byte)*Nbytes);
	int i = 0;
	for (i = 0; i < Nbytes; i++) bn->bytes[i] = 0;
    i++;
    bn->bytes[i] = '\0';
    bn->nbytes = Nbytes;
	return;
}

// Add two BigNums and store result in a third BigNum
void addBigNums (BigNum bnA, BigNum bnB, BigNum *res)
{
	// TODO
	char carry = 0;
	int nbytes = (bnA.nbytes > bnB.nbytes) ? bnA.nbytes : bnB.nbytes;
	res->nbytes = nbytes + 1;
	//res->bytes = realloc(res->bytes,sizeof(Byte)*(nbytes+1));
	
	for (int i = 0; i < nbytes; i++) {
	    res->bytes[i] = (bnA.bytes[i]+bnB.bytes[i]+carry) - '0';
	    carry = (bnA.bytes[i]+bnB.bytes[i] +1 > 10);
	    if (carry) res->bytes[i] -= 10;
    }
	
	return;
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum (char *s, BigNum *bn)
{
	// TODO
	if (strlen(s) < bn->nbytes) bn->nbytes = strlen(s);
	int i = 0;
	for (i = 0; i < bn->nbytes - 1; i++) {
	    if (!isdigit(s[i])) break;
	    bn->bytes[bn->nbytes-1-i] = s[i];
    }
    bn->bytes[i+1]='\0';
	return 1;
}

// Display a BigNum in decimal format
void showBigNum (BigNum bn)
{
	// TODO
	//while (!isdigit(s[i])) i++;
	for (int i = bn.nbytes - 1; i>=0; i--) {
	    printf("%c",bn.bytes[i]);
    }
    printf("\n");
	return;
}
