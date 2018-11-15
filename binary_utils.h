#pragma once

#include <iostream>

uint64_t get_bit(uint64_t value, int nr);
void set_bit(uint64_t *result, int nr, uint64_t value);
uint64_t permutate(uint64_t val, int *matrix, int length, int val_length);
uint64_t bits_cycle_left56(uint64_t val, const int rotations);
void split_bits56(uint64_t value, uint64_t *left, uint64_t *right);
void split_bits64(uint64_t value, uint64_t *left, uint64_t *right);
void print_bits(uint64_t value, int group_size, int length, std::string prefix);
void print_string_hex(char *value, uint64_t length, std::string prefix);
void print_hex(uint64_t value, std::string prefix);

uint64_t get_bit(uint64_t value, int nr)
{
    return (value >> nr) & 1;
}

void set_bit(uint64_t *result, int nr, uint64_t value)
{
    *result |= (value << nr);
}

uint64_t permutate(uint64_t val, int *matrix, int length, int val_length)
{
    uint64_t result = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        uint64_t bit = get_bit(val, val_length - matrix[i]);
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

void split_bits56(uint64_t value, uint64_t *left, uint64_t *right)
{
    *left = value >> 28;
    *right = (value << 36) >> 36;
}

void split_bits64(uint64_t value, uint64_t *left, uint64_t *right)
{
    *left = value >> 32;
    *right = (value << 32) >> 32;
}

void print_hex(uint64_t value, std::string prefix)
{
    std::cout << prefix << std::endl;
    std::cout << std::hex << value << std::endl; 
}

void print_string_hex(char *value, uint64_t length, std::string prefix)
{
    std::cout << prefix << std::endl;

    for(int i = 0; i < length; i++)
    {
        std::cout << std::hex << (int)value[i];
    }

    std::cout << "\n";
}

void print_bits(uint64_t value, int group_size, int length, std::string prefix)
{
    std::cout << prefix << std::endl;
    for (int i = length - 1; i >= 0; i--)
    {
        std::cout << get_bit(value, i);
        if (i % group_size == 0)
        {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}