#pragma once

#include <iostream>
#include <math.h>

#include "binary_utils.h"
#include "des.h"
#include "des_constant_cpu.h"

uint64_t create_pattern(uint64_t combination_number, char* alphabet, int32_t alphabet_length, int32_t segment_length);
uint64_t get_combinations_count(int alphabet_length, int input_length);

uint64_t create_pattern(uint64_t combination_number, char* alphabet, int32_t alphabet_length, int32_t value_length)
{
	int limit = 8 - value_length;
	
    uint64_t result = 0;
	for (int i = 8; --i >= limit;)
	{
		uint64_t y = combination_number / alphabet_length;
		result *= (1ULL << 8);
		result += alphabet[combination_number - y * alphabet_length];
		combination_number = y;
	}

	return result;
}

uint64_t get_combinations_count(int alphabet_length, int input_length)
{
	return pow(alphabet_length, input_length);
}

void prepare_data()
{
    char * key = "ddaac";
    int64_t key_hex = 0x6464616163;
    print_string_hex(key, 5, "Key:");

    char * message = "cca";
    int64_t message_hex = 0x636361;
    print_string_hex(message, 3, "Message:");

    uint64_t subkeyes[16];
    create_subkeyes(0x6464616162, subkeyes, cpu_SHIFTS, cpu_PC_1, cpu_PC_2);
    
    uint64_t ciphertext_v = des_encrypt(message_hex, subkeyes, cpu_IP, cpu_IP_REV, cpu_E_BIT, cpu_P, cpu_S);
    print_hex(ciphertext_v, "Ciphertext_hex:");
}