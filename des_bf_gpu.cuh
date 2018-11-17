#pragma once

#include <iostream>
#include <chrono>
#include <cstring>
#include <ctime>

#include "binary_utils.cuh"
#include "des.cuh"
#include "des_constant_cpu.cuh"
#include "des_utils.cuh"

__device__ void gpu_brute_force(char *key_alphabet, uint64_t key_alphabet_length, uint32_t key_length, char *message_alphabet, uint64_t message_alphabet_length,
    uint32_t message_length, uint64_t ciphertext, uint64_t *message_result, uint64_t *key_result, bool *found_key);
    
__host__ void des_brute_force_gpu(char *key_alphabet, int key_length, char *message_alphabet, int message_length, uint64_t ciphertext);


__device__ void gpu_brute_force(char *key_alphabet, uint64_t key_alphabet_length, uint32_t key_length, char *message_alphabet, uint64_t message_alphabet_length,
    uint32_t message_length, uint64_t ciphertext, uint64_t *message_result, uint64_t *key_result, bool *found_key)
{
    uint64_t keys_count = get_combinations_count(key_alphabet_length, key_length);
    uint64_t messages_cout = get_combinations_count(message_alphabet_length, message_length);
    uint64_t subkeyes[16];

    for (uint64_t i = 0; i < keys_count; i++)
    {
        uint64_t key = create_combination(i, key_alphabet, key_alphabet_length, key_length);
        // print_hex(key, "Key_" + std::to_string(i));
        create_subkeyes(key, subkeyes, cpu_SHIFTS, cpu_PC_1, cpu_PC_2);

        for (uint64_t j = 0; j < messages_cout; j++)
        {
            uint64_t message = create_combination(j, message_alphabet, message_alphabet_length, message_length);
            //print_hex(message, "Message_" + std::to_string(j));
            if (ciphertext == des_encrypt(message, subkeyes, cpu_IP, cpu_IP_REV, cpu_E_BIT, cpu_P, cpu_S))
            {
                *key_result = key;
                *message_result = message;
                *found_key = true;
                return;
            }
        }
    }
}

__host__ void des_brute_force_gpu(char *key_alphabet, int key_length, char *message_alphabet, int message_length, uint64_t ciphertext)
{
    std::cout << "DES GPU" << std::endl;
    uint64_t *key;
    uint64_t *message;
    bool *found_key;

    uint64_t key_alphabet_length = (uint64_t)std::strlen(key_alphabet);
    uint64_t message_alphabet_length = (uint64_t)std::strlen(message_alphabet);

    cudaMallocManaged(&key_alphabet, key_alphabet_length*sizeof(char));
    cudaMallocManaged(&message_alphabet, message_alphabet_length*sizeof(char));
    cudaMallocManaged(&message_alphabet, message_alphabet_length*sizeof(char));
    cudaMallocManaged(&key, sizeof(uint64_t));
    cudaMallocManaged(&message, sizeof(uint64_t));
    cudaMallocManaged(&found_key, sizeof(bool));

    std::chrono::steady_clock::time_point cpu_start, cpu_end;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    gpu_brute_force<<1, 1>>(key_alphabet, key_alphabet_length, key_length, message_alphabet, message_alphabet_length,
        message_length, ciphertext, message, key, found_key);

    cudaDeviceSynchronize();
    cudaFree(key_alphabet);
    cudaFree(message_alphabet);
    cudaFree(&key);
    cudaFree(&message);
    cudaFree(&found_key);
  
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    if (found_key)
    {
        print_hex(*key, "Found key:");
    }
    else
    {
        std::cout << "Key was not found" << std::endl;
    }

    std::cout << "Time elapsed:" << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;

    if(verify(*key, *message, ciphertext, cpu_IP, cpu_IP_REV, cpu_E_BIT, cpu_P, cpu_S, cpu_SHIFTS, cpu_PC_1, cpu_PC_2))
    {
        std::cout << "Verified OK." << std::endl;
    }
}
