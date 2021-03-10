
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <chrono>


#define BLOCKS 1024 * 1024
#define THREADS 256


__global__ void FindKey(uint64_t aPlainText, uint64_t aKeyMax, uint64_t aCipherExpected, uint64_t* aResult)
{
    int index = blockIdx.x * THREADS + threadIdx.x;
    int stride = BLOCKS * THREADS;

    if (*aResult)
    {
        return;
    }

    for (uint64_t key = index; key < aKeyMax; key += stride)
    {
        if (*aResult)
        {
            return;
        }

        uint64_t aWord = aPlainText;
        uint64_t aKey = key;

        for (char roundIndex = 0; roundIndex < 20; roundIndex++)
        {
            aWord ^= aKey;

            uint64_t sbox[] = { 0x2, 0x4, 0x5, 0x6, 0x1, 0xA, 0xF, 0x3, 0xB, 0xE, 0x0, 0x7, 0x9, 0x8, 0xC, 0xD };

            uint64_t wordNew = 0;

            for (char nibbleIndex = 0; nibbleIndex < 16; nibbleIndex++)
            {
                char nibble = (aWord >> (nibbleIndex * 4)) & 0xF;

                wordNew |= sbox[nibble] << (nibbleIndex * 4);
            }

            aWord = wordNew;

            aWord = ((aWord << 15) | (aWord >> (64 - 15))) ^ ((aWord << 32) | (aWord >> (64 - 32))) ^ aWord;

            aKey = ((aKey << 15) | (aKey >> (64 - 15))) ^ ((aKey << 32) | (aKey >> (64 - 32))) ^ aKey ^ 0x3;
        }

        if (aWord == aCipherExpected)
        {
            *aResult = key;
            return;
        }
    }
}


int main()
{
    // Assignment to find the key for
    // 0 - 20 bit key
    // 1 - 32 bit key
    // 2 - 44 bit key, cunldn't find :(
    char testCase = 2;

    uint64_t* resultHost;
    resultHost = (uint64_t*)malloc(sizeof(uint64_t));
    *resultHost = false;

    uint64_t* resultDevice;
    cudaMalloc((void**)&resultDevice, sizeof(uint64_t));
    cudaMemcpy(resultDevice, resultHost, sizeof(uint64_t), cudaMemcpyHostToDevice);

    // Find the key
    uint64_t plainTextVector[] =        { 0x0441E17A4283531C,   0x2E01B3D043EFA867, 0x176BF4819739A044  };
    uint64_t keyMaxVector[] =           { 0x0000000000100000,   0x0000000100000000, 0x0000100000000000  };
    uint64_t cipherExpectedVector[] =   { 0x4546788A5ADF2106,   0x839C3F5FD7CFA5E5, 0x3422F9DFE688E023  };

    auto start = std::chrono::high_resolution_clock::now();

    FindKey<<<BLOCKS, THREADS>>>(plainTextVector[testCase], keyMaxVector[testCase], cipherExpectedVector[testCase], resultDevice);

    cudaMemcpy(resultHost, resultDevice, sizeof(uint64_t), cudaMemcpyDeviceToHost);

    std::cout << "Key: " << std::hex << *resultHost << std::endl;

    std::cout << "RunTime: " << std::dec << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() << "[seconds]\n";

    return 0;
}
