#pragma once

#include <iostream>

#include "binary_utils.h"

uint64_t f(uint64_t right, uint64_t subkey, int *E_BIT, int *P, int **S);
void create_subkeyes(uint64_t key, uint64_t *subkeyes, int *SHIFTS, int *PC_1, int *PC_2);
uint64_t des_encrypt(uint64_t message, uint64_t *subkeyes, int *IP, int *IP_REV, int *E_BIT, int *P, int **S);

void create_subkeyes(uint64_t key, uint64_t *subkeyes, int *SHIFTS, int *PC_1, int *PC_2)
{
    // print_bits(key, 8, 64, "Key: ");
    uint64_t k_plus = permutate(key, PC_1, 56, 64);
    // print_bits(k_plus, 7, 64, "K+: ");

    uint64_t c_0, d_0;
    split_bits56(k_plus, &c_0, &d_0);
    // print_bits(c0, 28, 28, "C_0: ");
    // print_bits(d0, 28, 28, "D_0: ");

    uint64_t c_tmp, d_tmp, cd_tmp, c_prev = c_0, d_prev = d_0;

    for (int i = 1; i <= 16; i++)
    {
        c_tmp = bits_cycle_left56(c_prev, SHIFTS[i - 1]);
        d_tmp = bits_cycle_left56(d_prev, SHIFTS[i - 1]);
        c_prev = c_tmp;
        d_prev = d_tmp;

        cd_tmp = c_tmp << 28 | d_tmp;
        subkeyes[i - 1] = permutate(cd_tmp, PC_2, 48, 56);
        // print_bits(subkeyes[i - 1], 48, 48, "Subkey " + std::to_string(i));
    }
}

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

uint64_t des_encrypt(uint64_t message, uint64_t *subkeyes, int *IP, int *IP_REV, int *E_BIT, int *P, int **S)
{
    // print_bits(message, 4, 64, "Message: ");
    uint64_t ip = permutate(message, IP, 64, 64);
    // print_bits(ip, 4, 64, "IP: ");

    uint64_t left, right;
    split_bits64(ip, &left, &right);
    // print_bits(left, 4, 32, "L_0");
    // print_bits(right, 4, 32, "R_0");

    uint64_t left_prev, right_prev;
    for (int i = 0; i < 16; i++)
    {
        left_prev = left;
        right_prev = right;
        left = right_prev;
        right = left_prev ^ f(right_prev, subkeyes[i], E_BIT, P, S);
        // print_bits(right, 4, 64, "R_" + std::to_string(i + 1));
    }

    uint64_t rl = right << 32 | left;
    //print_bits(rl, 4, 64, "RL");
    uint64_t encrypted = permutate(rl, IP_REV, 64, 64);

    return encrypted;
}