#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sieve.h"

const ullong max_number = 100000000;

int main ()
{
	uint sieve_size = (uint)sqrt ((double)max_number) + 1;
	byte *sieve = create_sieve (sieve_size);
    printf ("Sieve created\n");
	for (ullong number = 2; number <= max_number; number++)
        if (is_prime(sieve, sieve_size, number) != is_prime_mr(number))
        {
            printf ("Error in number: %llu\n", number);
            return 1;
        }
    printf ("\nMiller-Rabin works correctly\n");
	return 0;
}
