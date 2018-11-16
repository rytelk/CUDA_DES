#include <iostream>
#include <sstream>
#include <math.h>

#include "usage.h"
#include "binary_utils.h"
#include "des_constant_cpu.h"
#include "des.h"
#include "des_bf_cpu.h"

int main(void)
{
    usage();
    bool useCpu = true;
    bool useGpu = false;

    int key_length = 5;
    char *key_alphabet = "abcdefgh";
    print_string_hex(key_alphabet, 5, "Key alphabet:");

    int message_length = 3;
    char *message_alphabet = "abc";
    print_string_hex(message_alphabet, 3, "Message alphabet:");

    uint64_t ciphertext = 0xb7ec1a731f8806f8;
    print_hex(ciphertext, "Ciphertext_hex:");

    if (useCpu)
    {
        des_brute_force_cpu(key_alphabet, key_length, message_alphabet, message_length, ciphertext);
    }
    else if (useGpu)
    {
        // TODO: GPU
    }
    
    return 0;
}