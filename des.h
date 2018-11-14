#pragma once

#include <iostream>

#include "des_utils.h"
#include "binary_utils.h"

uint64_t f(uint64_t right, uint64_t subkey, int *E_BIT, int *P, int **S);

uint64_t f(uint64_t right, uint64_t subkey, int *E_BIT, int *P, int **S)
{
    //print_bits(right, 4, 32, "R");
    right = permutate(right, E_BIT, 48, 32);
    //print_bits(right, 6, 48, "E(R)");
    right ^= subkey;
    //print_bits(right, 6, 48, "XORED R");

    int i = 7;
    uint64_t result = 0;
    
    for (int i = 0; i < 8; i++)
    {
        //print_bits(right, 6, 64, "R");
        uint64_t left_tmp = (right << (16 + 6 * i)) >> 63;
        uint64_t right_tmp = (right << (21 + 6 * i)) >> 63;

        //print_bits(left_tmp, 6, 6, "left_tmp_" + std::to_string(i));
        //print_bits(right_tmp, 6, 6, "right_tmp_" + std::to_string(i));
        uint64_t outer = left_tmp << 1 | right_tmp;
        //print_bits(outer, 2, 2, "outer_" + std::to_string(i));

        uint64_t inner = (right << (17 + 6 * i)) >> 60;
        //print_bits(inner, 4, 4, "inner_" + std::to_string(i));
        uint64_t sbox_value = S[i][outer << 4 | inner];
        //print_bits(sbox_value, 4, 64, "sbox_value_" + std::to_string(i));
        result ^= sbox_value << (28 - 4 * i);
        //print_bits(result, 4, 64, "result_" + std::to_string(i));
    }

    //print_bits(result, 4, 64, "Esses");
    result = permutate(result, P, 32, 32);
    //print_bits(result, 4, 32, "Permuatated esses");

    return result;
}