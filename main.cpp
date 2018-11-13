#include <iostream>
#include <math.h>

#include "usage.h"
#include "binary_utils.h"
#include "des_constant_cpu.h"
#include "des_utils.h"

int main(void)
{
    usage();
    int key_length = 32;
    uint64_t message = 0x0123456789ABCDEF;
    uint64_t key = 0x133457799BBCDFF1;
    print_bits(key, 8, 64);
    uint64_t permutated = bits_permutate(key, cpu_PC_1, 56, 64);
    print_bits(permutated, 7, 64);
    /*uint64_t key = des_generate_key_length(key_length);
	uint64_t block = 0x0123456789ABCDEF;
	uint64_t encoded = full_des_encode_block(key, block); */
    return 0;
}