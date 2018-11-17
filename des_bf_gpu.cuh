#pragma once

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <cstring>
#include <ctime>

#include "binary_utils.cuh"
#include "des.cuh"
#include "des_constant_cpu.cuh"
#include "des_constant_gpu.cuh"
#include "des_utils.cuh"

__global__ void gpu_brute_force(char *key_alphabet, int64_t key_alphabet_length, int key_length, char *message_alphabet, int64_t message_alphabet_length,
    int message_length, uint64_t ciphertext, uint64_t *message_result, uint64_t *key_result, bool *found_key);
    
__host__ void des_brute_force_gpu(char *key_alphabet, int key_length, char *message_alphabet, int message_length, uint64_t ciphertext);


__global__ void gpu_brute_force(char *key_alphabet, int64_t key_alphabet_length, int key_length, char *message_alphabet, int64_t message_alphabet_length,
                     int message_length, uint64_t ciphertext, uint64_t *message_result, uint64_t *key_result, bool *found_key)
{
    uint64_t keys_count = get_combinations_count(key_alphabet_length, key_length);
    uint64_t messages_cout = get_combinations_count(message_alphabet_length, message_length);
    uint64_t subkeyes[16];

    printf("\n--- GPU PARAMS --- \n");
    printf("key_alphabet %s\n", key_alphabet);
    printf("key_alphabet_length %d\n", key_alphabet_length);
    printf("key_length %d\n", key_length);
    printf("message_alphabet %s\n", message_alphabet);
    printf("message_alphabet_length %d\n", message_alphabet_length);
    printf("message_length %d\n", message_length);
    printf("ciphertext 0x%016x\n", ciphertext);
    printf("--- END PARAMS --- \n\n");

    for (uint64_t i = 0; i < keys_count; i++)
    {
        uint64_t key = create_combination(i, key_alphabet, key_alphabet_length, key_length);
        printf("Key %d 0x%016x\n", i, key);
        //print_hex(key, "Key_" + std::to_string(i));
        create_subkeyes(key, subkeyes, gpu_SHIFTS, gpu_PC_1, gpu_PC_2);

        for (uint64_t j = 0; j < messages_cout; j++)
        {
            uint64_t message = create_combination(j, message_alphabet, message_alphabet_length, message_length);
            printf("Message %d 0x%016x\n", i, message);
            //print_hex(message, "Message_" + std::to_string(j));
            uint64_t test_cipher = des_encrypt(message, subkeyes, gpu_IP, gpu_IP_REV, gpu_E_BIT, gpu_P, gpu_S);
            printf("0x%016x\n", test_cipher);            
            if (ciphertext == test_cipher)
            {
                *key_result = key;
                *message_result = message;
                *found_key = true;
                printf("Key found GPU\n");\
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

    int64_t key_alphabet_length = (int64_t)std::strlen(key_alphabet);
    int64_t message_alphabet_length = (int64_t)std::strlen(message_alphabet);

    char *gpu_key_alphabet, *gpu_message_alphabet;
     
    cudaMallocManaged(&gpu_key_alphabet, key_alphabet_length*sizeof(char));
    cudaMallocManaged(&gpu_message_alphabet, message_alphabet_length*sizeof(char));
    cudaMallocManaged(&key, sizeof(uint64_t));
    cudaMallocManaged(&message, sizeof(uint64_t));
    cudaMallocManaged(&found_key, sizeof(bool));

    cudaError_t cudaStatus1 = cudaMemcpy(gpu_key_alphabet, key_alphabet, key_alphabet_length, cudaMemcpyHostToDevice);
    cudaError_t cudaStatus2 = cudaMemcpy(gpu_message_alphabet, message_alphabet, message_alphabet_length, cudaMemcpyHostToDevice);

    if (cudaStatus1 != cudaSuccess) {
		printf("%s\n", cudaGetErrorString(cudaStatus1));
    }
    
    if (cudaStatus2 != cudaSuccess) {
		printf("%s\n", cudaGetErrorString(cudaStatus2));
	}

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    gpu_brute_force<<<1, 1>>>(
        gpu_key_alphabet, 
        key_alphabet_length, 
        key_length, 
        gpu_message_alphabet, 
        message_alphabet_length,
        message_length, 
        ciphertext, 
        message, 
        key, 
        found_key);

    cudaDeviceSynchronize();
  
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    if (*found_key)
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

    cudaFree(key_alphabet);
    cudaFree(message_alphabet);
    cudaFree(&key);
    cudaFree(&message);
    cudaFree(&found_key);
}
