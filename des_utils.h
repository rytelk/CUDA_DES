#include <iostream>

#include "binary_utils.h"

uint64_t bits_permutate(uint64_t key, int *matrix, int length, int key_length);

uint64_t bits_permutate(uint64_t key, int *matrix, int length, int key_length)
{
    uint64_t result = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        uint64_t bit = get_bit(key, key_length - matrix[i]);
        set_bit(&result, length - 1 - i, bit);
    }
    return result;
}