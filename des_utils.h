#include <iostream>

#include "binary_utils.h"

uint64_t permutate(uint64_t key, int *matrix, int length, int key_length);
uint64_t bits_cycle_left56(uint64_t val, const int rotations);

uint64_t permutate(uint64_t key, int *matrix, int length, int key_length)
{
    uint64_t result = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        uint64_t bit = get_bit(key, key_length - matrix[i]);
        set_bit(&result, length - 1 - i, bit);
    }
    return result;
}

uint64_t bits_cycle_left56(uint64_t val, const int rotations)
{
    uint64_t result = val;
    uint64_t left, right;
    result <<= rotations;
    split_bits56(result, &left, &right);
    result |= left;
    result = (result << 36) >> 36;
    return result;
}