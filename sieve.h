#pragma once

typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long long ullong;

// Miller-Rabin test
bool is_prime_mr (ullong number);

// functions for work with sieve
byte *create_sieve (ullong sieve_size);
bool is_prime_in_sieve (const byte *sieve, ullong sieve_size, ullong n);
bool is_prime (const byte *sieve, ullong sieve_size, ullong n);
uint find_quantity (const byte *sieve, ullong sieve_size);
ullong find_n_prime (const byte *sieve, ullong sieve_size, uint n);
void free_sieve (byte *sieve);

// functions for work with prime table
ullong *create_prime_table (const byte *sieve, ullong sieve_size, uint *prime_table_size);
void free_prime_table (ullong *prime_table);
