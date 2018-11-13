#pragma once
#include <iostream>

uint64_t get_bit(uint64_t value, int nr);
void set_bit(uint64_t *result, int nr, uint64_t value);
void print_bits(uint64_t value, int group_size, int length);

uint64_t get_bit(uint64_t value, int nr)
{
    return (value >> nr) & 1;
}

void set_bit(uint64_t *result, int nr, uint64_t value)
{
    *result |= (value << nr);
}

void split_bits56(uint64_t value, uint64_t *left, uint64_t *right)
{
    *left = value >> 28;
    *right = (value << 36) >> 36;
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