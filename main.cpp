#include <iostream>
#include <math.h>

#include "usage.h"
#include "binary_utils.h"
#include "des_constant_cpu.h"
#include "des_utils.h"

int main(void)
{
    usage();
    int key_length = 64;
    uint64_t message = 0x0123456789ABCDEF;
    uint64_t key = 0x133457799BBCDFF1;

    // Stage 1

    print_bits(key, 8, 64, "Key: ");
    uint64_t k_plus = permutate(key, cpu_PC_1, 56, 64);
    // print_bits(k_plus, 7, 64, "K+: ");

    uint64_t c0, d0;
    split_bits56(k_plus, &c0, &d0);
    // print_bits(c0, 28, 28, "C_0: ");
    // print_bits(d0, 28, 28, "D_0: ");

    uint64_t subkeyes[16];
    uint64_t c_tmp, d_tmp, cd_tmp, c_prev = c0, d_prev = d0;
    for (int i = 1; i <= 16; i++)
    {
        c_tmp = bits_cycle_left56(c_prev, cpu_SHIFTS[i - 1]);
        d_tmp = bits_cycle_left56(d_prev, cpu_SHIFTS[i - 1]);
        c_prev = c_tmp;
        d_prev = d_tmp;

        cd_tmp = c_tmp << 28 | d_tmp;
        subkeyes[i - 1] = permutate(cd_tmp, cpu_PC_2, 48, 56);
        // print_bits(subkeyes[i - 1], 48, 48, "Subkey " + std::to_string(i));
    }

    // Stage 2



    return 0;
}