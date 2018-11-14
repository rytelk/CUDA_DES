#pragma once

#include <iostream>

#include "des_utils.h"
#include "binary_utils.h"

uint64_t f(uint64_t right, uint64_t subkey, int *E_BIT, int *P, int **S);

uint64_t f(uint64_t right, uint64_t subkey, int *E_BIT, int *P, int **S)
{
    print_bits(right, 4, 32, "R_0");
    right = permutate(right, E_BIT, 48, 32);
    print_bits(right, 6, 48, "E(R_0)");
    right ^= subkey;

    uint64_t result = 0;
    for (int i = 0; i < 8; i++)
    {
        int left_tmp = (right & (1ULL << (63 - 6 * i))) >> (63 - 6 * i);
        int right_tmp = (right & (1ULL << (58 - 6 * i))) >> (58 - 6 * i);

        int outer = left_tmp << 1 | right_tmp;
        int inner = (right & (0xFULL << (59 - 6 * i))) >> (59 - 6 * i);

        uint64_t piece = S[i][outer << 4 | inner];

        result ^= piece << (60 - 4 * i);
    }

    return permutate(result, P, 32, 32);
}