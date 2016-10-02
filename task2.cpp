#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include "sieve.h"

const uint digits = 7;
const uint radix = 10;

const uint fact[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600 };

bool check_is_prime (ullong number, const ullong *prime_table, uint prime_table_size)
{
	if (number == 1)
		return false;
	for (uint i = 0; i < prime_table_size; i++)
		if (!(number % prime_table[i]))
			return false;
	return true;
}

void swap (byte *a, byte *b)
{
	byte t = *a;
	*a = *b;
	*b = t;
}

void reverse (byte *left, byte *right)
{
	while (left < right)
	{
		swap (left, right);
		left++;
		right--;
	}
}

void gen_prev_number (byte *number, uint n)
{
	for (uint i = n - 1; i>=1; i--)
		if (number[i - 1] > number[i])
		{
			byte max = i;
			for (uint j = i; j < n; j++)
				if (number[j] > number[max] && number[j] < number[i - 1])
					max = j;
			swap (number + i - 1, number + max);
			reverse (number + i, number + n - 1);
			return;
		}
}

uint number_to_uint (const byte *number, uint n)
{
	uint ans = 0;
	for (uint i = 0; i < n; i++)
	{
		assert (ans < UINT_MAX / radix);
		ans = ans * radix + number[i];
	}
	return ans;
}

int main ()
{
	assert (digits < radix);
	byte *number = (byte *)calloc (digits, 1);
	assert (number);
	for (uint i = 0; i < digits; i++)
		number[i] = digits - i;
	uint max_divider = (uint)sqrt ((double)number_to_uint (number, digits));
	byte *sieve = create_sieve (max_divider);
	uint prime_table_size;
	ullong *prime_table = create_prime_table (sieve, max_divider, &prime_table_size);
	bool is_answer_find = false;
	for (uint i = 0; i < fact[digits] - 1; i++)
	{
		uint cur_number = number_to_uint (number, digits);
		if (check_is_prime (cur_number, prime_table, prime_table_size))
		{
			is_answer_find = true;
			printf ("Prime number: %u\n", cur_number);
			break;
		}
		gen_prev_number (number, digits);
	}
	if (!is_answer_find)
		printf ("Prime number not found\n");
	free_prime_table (prime_table);
	free_sieve (sieve);
	free (number);
	return 0;
}
