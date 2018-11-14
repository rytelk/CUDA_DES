#pragma once

#include <iostream>

uint64_t get_bit(uint64_t value, int nr);
void set_bit(uint64_t *result, int nr, uint64_t value);
void print_bits(uint64_t value, int group_size, int length, std::string prefix);
void print_hex(uint64_t value, std::string prefix);

uint64_t get_bit(uint64_t value, int nr)
{
    return (value >> nr) & 1;
}

void set_bit(uint64_t *result, int nr, uint64_t value)
{
    *result |= (value << nr);
}

void print_hex(uint64_t value, std::string prefix)
{
    std::cout << prefix << std::endl;
    std::cout << std::hex << value << std::endl; 
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