#include <iostream>
#include <chrono>


uint64_t RotateLeft(uint64_t aWord, char aN)
{
    return (aWord << aN) | (aWord >> (64 - aN));
}


uint64_t Linear(uint64_t aWord)
{
    return RotateLeft(aWord, 15) ^ RotateLeft(aWord, 32) ^ aWord;
}


uint64_t ApplySbox(uint64_t aWord)
{
    uint64_t sbox[] = { 0x2, 0x4, 0x5, 0x6, 0x1, 0xA, 0xF, 0x3, 0xB, 0xE, 0x0, 0x7, 0x9, 0x8, 0xC, 0xD };

    uint64_t wordNew = 0;

    for (char nibbleIndex = 0; nibbleIndex < 16; nibbleIndex++)
    {
        char nibble = (aWord >> (nibbleIndex * 4)) & 0xF;

        wordNew |= sbox[nibble] << (nibbleIndex * 4);
    }

    return wordNew;
}


uint64_t Encrypt(uint64_t aWord, uint64_t aKey)
{
    for (char roundIndex = 0; roundIndex < 20; roundIndex++)
    {
        aWord ^= aKey;

        aWord = ApplySbox(aWord);

        aWord = Linear(aWord);

        aKey = Linear(aKey) ^ 0x3;
    }

    return aWord;
}


bool VerifyTestVectors()
{
    uint64_t plainTextTestVector[] =        { 0x0000000000000000, 0x1234567890ABCDEF };
    uint64_t keyTestVector[] =              { 0x0000000000000000, 0x1234567890ABCDEF };
    uint64_t cipherExpectedTestVector[] =   { 0x33F88BFC146EF748, 0xB9AE78D22D338F55 };

    for (char testVectorIndex = 0; testVectorIndex < 2; testVectorIndex++)
    {
        uint64_t cipher = Encrypt(plainTextTestVector[testVectorIndex], keyTestVector[testVectorIndex]);

        if (cipher != cipherExpectedTestVector[testVectorIndex])
        {
            return false;
        }
    }

    return true;
}


int main()
{
    // Assignment to find the key for
    // 0 - 20 bit key
    // 1 - 32 bit key
    // 2 - 44 bit key, cunldn't find :(
    char testCase = 0;

    // Check test vectors for sanity
    if (!VerifyTestVectors())
    {
        std::cout << "VerifyTestVectors Failed !!!" << std::endl;
        return 0;
    }

    // Find the key
    uint64_t plainTextVector[] =        { 0x0441E17A4283531C,   0x2E01B3D043EFA867, 0x176BF4819739A044 };
    uint64_t cipherExpectedVector[] =   { 0x4546788A5ADF2106,   0x839C3F5FD7CFA5E5, 0x3422F9DFE688E023 };
    uint64_t keyExpectedVector[] =      { 0x76F4D,              0x30F7B078,         0x0                };
    uint64_t keyMaxVector[] =           { 0x0000000000100000,   0x0000000100000000, 0x0000100000000000 };

    uint64_t plainText = plainTextVector[testCase];
    uint64_t keyMax = keyMaxVector[testCase];
    uint64_t cipherExpected = cipherExpectedVector[testCase];

    auto start = std::chrono::high_resolution_clock::now();

    for (uint64_t key = 0; key < keyMax; key++)
    {
        uint64_t cipher = Encrypt(plainText, key);

        if (cipher == cipherExpected)
        {
            std::cout << "Key: " << std::hex << key << std::endl;
            if (key != keyExpectedVector[testCase])
            {
                std::cout << "Found key differnet than expected !!!" << std::endl;
            }
            break;
        }
    }

    std::cout << "RunTime: " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count() << "[seconds]\n";

    return 0;
}
