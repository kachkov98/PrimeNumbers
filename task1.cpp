#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const unsigned int prime_number = 50;

// convert sieve index to number; numbers are of the form 6k-1, 6k+1
#define SIEVE_INDEX_TO_NUMBER(i) ( (((i) / 2) + 1) * 6 + ((i) % 2) * 2 - 1)
// check bit i in sieve (if 0)
#define CHECK_PRIME(i) (!(sieve[(i)/8] & (1 << ((i) % 8))))
// set bit i as not prime (to 1)
#define SET_NOT_PRIME(i) (sieve[(i)/8] = sieve[(i)/8] | (1 << ((i) % 8)))

// build sieve
void build_sieve (char *sieve, size_t sieve_size)
{
	unsigned int max_number = SIEVE_INDEX_TO_NUMBER (sieve_size * 8 - 1);
	unsigned int sqrt_max_number = (unsigned int)sqrt ((double)max_number);
	for (unsigned int i = 0; i < sqrt_max_number; i++)
		if (CHECK_PRIME (i))
		{
			int cur_number = SIEVE_INDEX_TO_NUMBER (i);
			for (unsigned int j = 2 * cur_number; j <= max_number; j += cur_number)
			{
				// set number j in sieve not prime
				if (j % 6 == 1 || j % 6 == 5)
					SET_NOT_PRIME (j / 3 - 1);
			}
		}
}

// find prime in sieve
unsigned int find_prime (char *sieve, size_t sieve_size)
{
	assert (prime_number);
	switch (prime_number)
	{
	case 1:
		return 2;
	case 2:
		return 3;
	}
	
	unsigned int max_number = SIEVE_INDEX_TO_NUMBER (sieve_size * 8 - 1);
	unsigned int cur_prime_number = 2;
	for (unsigned int i = 0; i < max_number; i++)
		if (CHECK_PRIME (i))
			if (++cur_prime_number == prime_number)
				return SIEVE_INDEX_TO_NUMBER (i);
	printf ("Sieve size not enough!");
	return 0;
}

size_t max (size_t a, size_t b)
{
	return (a > b) ? a : b;
}

int main (int argc, char *argv[])
{
	// minimum size of sieve is enough for 20 prime number
	size_t sieve_size = max (3, (size_t)prime_number * (log(prime_number) + log(log (prime_number))) / 24);
	char *sieve = (char *)calloc (sieve_size, 1); // array for numbers 6*k-1 and 6*k+1; k>=1
	assert (sieve);
	build_sieve (sieve, sieve_size);
	printf ("%u\n", find_prime (sieve, sieve_size));
	free (sieve);
	return 0;
}
