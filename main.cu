#include <iostream>
#include <sstream>
#include <math.h>

#include "usage.cuh"
#include "binary_utils.cuh"
#include "des_constant_cpu.cuh"
#include "des.cuh"
#include "des_bf_cpu.cuh"
#include "des_bf_gpu.cuh"

int main(void)
{
    usage();
    bool useCpu = false;
    bool useGpu = true;

    int key_length = 1;
    char *key_alphabet = "a";
    print_string_hex(key_alphabet, 1, "Key alphabet:");

    int message_length = 1;
    char *message_alphabet = "a";
    print_string_hex(message_alphabet, 1, "Message alphabet:");

    uint64_t ciphertext = 0x9e79415222426c15;
    print_hex(ciphertext, "Ciphertext_hex:");

    if (useCpu)
    {
        des_brute_force_cpu(key_alphabet, key_length, message_alphabet, message_length, ciphertext);
    }
    if (useGpu)
    {
        des_brute_force_gpu(key_alphabet, key_length, message_alphabet, message_length, ciphertext);
    }

    return 0;
}

/*
    int key_length = 5;
    char *key_alphabet = "abcdefgh";
    print_string_hex(key_alphabet, 5, "Key alphabet:");

    int message_length = 3;
    char *message_alphabet = "abc";
    print_string_hex(message_alphabet, 3, "Message alphabet:");

    uint64_t ciphertext = 0xb7ec1a731f8806f8;
    print_hex(ciphertext, "Ciphertext_hex:");
*/