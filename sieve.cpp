#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include "sieve.h"

#define GET_BIT(i) (sieve[(i) / 8] & (1 << ((i) % 8)))
#define SET_BIT(i) (sieve[(i) / 8] = sieve[(i) / 8] | (1 << ((i) % 8)))
#define SIEVE_SIZE_TO_NUMBER_OF_BITS(n) (ullong)((n + 1) / 6 * 2 - ((n % 6 == 5 || n % 6 == 0) ? 1 : 0))
#define SIEVE_INDEX_TO_NUMBER(i) ((i / 2 + 1) * 6 + ((i % 2) ? 1 : -1))

// Miller-Rabin test
ullong modular_multiplication (ullong a, ullong b, ullong m)
{
    if (a <= ULONG_MAX / (b + 1))
        return (a * b) % m;
    if ((a % m) <= ULONG_MAX / ((b % m) + 1))
        return ((a % m) * (b % m)) % m;
    ullong ans = 0;
    while (b)
    {
        if (b % 2)
            ans = (ans + a) % m;
        b /= 2;
        a = (a * 2) % m;
    }
    return ans;
}

ullong modular_exponentiation (ullong a, ullong e, ullong m)
{
	ullong ans = 1;
	while (e)
	{
		if (e % 2)
            ans = modular_multiplication(ans, a, m);
        e /= 2;
        a = modular_multiplication(a, a, m);
	}
	return ans;
}

ullong pow2 (byte e)
{
	return ((ullong)1 << e);
}

bool is_prime_mr (ullong number)
{
    if (number < 2)
        return false;
    if (number == 2)
        return true;
    if (!(number % 2))
		return false;
	ullong d = number - 1;
	byte s = 0;
	while (!(d % 2))
	{
		d /= 2;
		s++;
	}
	
	const uint w1[]  = {2};
    const uint w2[]  = {2, 3};
    const uint w3[]  = {31, 73};
    const uint w4[]  = {2, 3, 5};
    const uint w5[]  = {2, 3, 5, 7};
    const uint w6[]  = {2, 7, 61};
    const uint w7[]  = {2, 13, 23, 1662803};
    const uint w8[]  = {2, 3, 5, 7, 11};
    const uint w9[]  = {2, 3, 5, 7, 11, 13};
    const uint w10[] = {2, 3, 5, 7, 11, 13, 17};
    const uint w11[] = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    const uint w12[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    
	const uint *witnesses;
    uint witnesses_len;
    #define SET_WITNESSES(name) {witnesses = name; witnesses_len = sizeof(name) / sizeof (uint);}
	if (number < 2047)
        SET_WITNESSES(w1)
    else if (number < 1373653)
        SET_WITNESSES(w2)
    else if (number < 9080191)
        SET_WITNESSES(w3)
    else if (number < 25326001)
        SET_WITNESSES(w4)
    else if (number < 3215031751)
        SET_WITNESSES(w5)
    else if (number < 4759123141)
        SET_WITNESSES(w6)
    else if (number < 1122004669633)
        SET_WITNESSES(w7)
    else if (number < 2152302898747)
        SET_WITNESSES(w8)
    else if (number < 3474749660383)
        SET_WITNESSES(w9)
    else if (number < 341550071728321)
        SET_WITNESSES(w10)
    else if (number < 3825123056546413051)
        SET_WITNESSES(w11)
    else
        SET_WITNESSES(w12)
    #undef SET_WITNESSES
        
	for (byte i = 0; i < witnesses_len; i++)
	{
		bool non_prime_sign = true;
		for (byte j = 0; j < s; j++)
			if (modular_exponentiation ((ullong)witnesses[i], pow2 (j) * d, number) == number - 1)
			{
				non_prime_sign = false;
				break;
			}
		if (non_prime_sign && modular_exponentiation ((ullong)witnesses[i], d, number) != 1)
            return false;
	}
	return true;
}

// functions for work with sieve
byte *create_sieve (ullong sieve_size)
{
	ullong size_in_bits = SIEVE_SIZE_TO_NUMBER_OF_BITS (sieve_size);
	byte *sieve = (byte*)calloc ((uint)(size_in_bits / 8 + ((size_in_bits % 8) ? 1 : 0)), 1);
	assert (sieve);

	uint sqrt_size_in_bits = (uint)SIEVE_SIZE_TO_NUMBER_OF_BITS ((ullong)sqrt ((double)sieve_size));
	for (uint i = 0; i < sqrt_size_in_bits; i++)
		if (!GET_BIT (i))
		{
			ullong number = SIEVE_INDEX_TO_NUMBER (i);
			for (ullong j = 2 * number; j <= sieve_size; j += number)
				if (j % 6 == 1 || j % 6 == 5)
					SET_BIT (j / 3 - 1);
		}
	return sieve;
}

bool is_prime_in_sieve (const byte *sieve, ullong sieve_size, ullong n)
{
	assert (sieve);
	assert (n <= sieve_size);
	if (n == 0 || n == 1)
		return false;
	if (n == 2 || n == 3)
		return true;
	if (n % 6 == 1 || n % 6 == 5)
		return !GET_BIT (n / 3 - 1);
	else
		return false;
}

bool is_prime (const byte *sieve, ullong sieve_size, ullong n)
{
	assert (sieve);
	assert (n <= sieve_size * sieve_size);
	if (n == 0 || n == 1)
		return false;
	if (n == 2 || n == 3 || n == 5 || n == 7)
		return true;
	if (n % 6 == 1 || n % 6 == 5)
	{
		uint sqrt_size = (uint)sqrt ((double)n);
		for (uint i = 0; i <= SIEVE_SIZE_TO_NUMBER_OF_BITS (sqrt_size); i++)
			if (!GET_BIT (i) && !(n % SIEVE_INDEX_TO_NUMBER (i)))
				return false;
		return true;
	}
	else
		return false;
}

uint find_quantity (const byte *sieve, ullong sieve_size)
{
	assert (sieve);
	if (sieve_size == 2) return 1;
	if (sieve_size == 3) return 2;
	uint n = 2;
	for (ullong i = 0; i < SIEVE_SIZE_TO_NUMBER_OF_BITS (sieve_size); i++)
		if (!GET_BIT (i))
			n++;
	return n;
}

ullong find_n_prime (const byte *sieve, ullong sieve_size, uint n)
{
	assert (sieve);
	if (n == 1) return 2;
	if (n == 2) return 3;
	uint cur_n = 2;
	for (ullong i = 0; i < SIEVE_SIZE_TO_NUMBER_OF_BITS (sieve_size); i++)
		if (!GET_BIT (i) && ++cur_n == n)
			return SIEVE_INDEX_TO_NUMBER (i);
	assert ("Sieve doesn't contain prime this number! Try to increase size of sieve");
	exit (EXIT_FAILURE);
}

void free_sieve (byte *sieve)
{
	free (sieve);
}

// functions for work with prime table
ullong *create_prime_table (const byte *sieve, ullong sieve_size, uint *prime_table_size)
{
	*prime_table_size = find_quantity (sieve, sieve_size);
	ullong *prime_table = (ullong *)calloc (*prime_table_size, sizeof (ullong));
	assert (prime_table);
	if (*prime_table_size > 0)
		prime_table[0] = 2;
	if (*prime_table_size > 1)
		prime_table[1] = 3;
	uint cur_n = 2;
	for (ullong i = 0; i < SIEVE_SIZE_TO_NUMBER_OF_BITS (sieve_size); i++)
		if (!GET_BIT (i))
			prime_table[cur_n++] = SIEVE_INDEX_TO_NUMBER (i);
	return prime_table;
}

void free_prime_table (ullong *prime_table)
{
	free (prime_table);
}
