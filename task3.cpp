#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sieve.h"

const ullong number = 72057594046317541;//72057594046316545;

int main ()
{
	uint sieve_size = (uint)sqrt ((double)number) + 1;
	byte *sieve = create_sieve (sieve_size);
	printf ("Sieve test: ");
	if (is_prime (sieve, sieve_size, number))
		printf ("Number %llu is prime\n", number);
	else
		printf ("Number %llu is not prime\n", number);
	free_sieve (sieve);
	printf ("Miller-Rabin test: ");
	if (is_prime_mr (number))
		printf ("Number %llu is prime\n", number);
	else
		printf ("Number %llu is not prime\n", number);
	return 0;
}
