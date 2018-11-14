#include <iostream>
#include <math.h>

#include "usage.h"
#include "binary_utils.h"
#include "des_constant_cpu.h"
#include "des_utils.h"
#include "des.h"

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

    uint64_t c_0, d_0;
    split_bits56(k_plus, &c_0, &d_0);
    // print_bits(c0, 28, 28, "C_0: ");
    // print_bits(d0, 28, 28, "D_0: ");

    uint64_t subkeyes[16];
    uint64_t c_tmp, d_tmp, cd_tmp, c_prev = c_0, d_prev = d_0;
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

    print_bits(message, 4, 64, "Message: ");
    uint64_t ip = permutate(message, cpu_IP, 64, 64);
    print_bits(ip, 4, 64, "IP: ");
    
    uint64_t left, right;
    split_bits64(ip, &left, &right);
    print_bits(left, 4, 32, "L_0");
    print_bits(right, 4, 32, "R_0");

    uint64_t left_prev, right_prev;
    for (int i = 0; i < 16; i++)
    {
        left_prev = left;
		right_prev = right;
		left = right_prev;
		right = left_prev ^ f(right_prev, subkeyes[i], cpu_E_BIT, cpu_P, cpu_S);
    }

    return 0;
}