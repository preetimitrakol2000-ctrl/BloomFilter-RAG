#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FILTER_SIZE_BITS 1024

typedef struct {
    unsigned char storage_bytes[FILTER_SIZE_BITS / 8];
} BloomFilter;

#ifdef _WIN32
    __declspec(dllexport) BloomFilter* init_bloom();
    __declspec(dllexport) void register_token(BloomFilter* bf, const char* token);
    __declspec(dllexport) bool check_token_presence(BloomFilter* bf, const char* token);
#endif

unsigned int hash_alpha(const char* str) {
    unsigned int hash = 5381;
    while (*str) hash = ((hash << 5) + hash) + *str++;
    return hash % FILTER_SIZE_BITS;
}

unsigned int hash_beta(const char* str) {
    unsigned int hash = 0;
    while (*str) hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    return hash % FILTER_SIZE_BITS;
}

BloomFilter* init_bloom() {
    BloomFilter* bf = (BloomFilter*)malloc(sizeof(BloomFilter));
    memset(bf->storage_bytes, 0, sizeof(bf->storage_bytes));
    return bf;
}

void register_token(BloomFilter* bf, const char* token) {
    unsigned int bit_a = hash_alpha(token);
    unsigned int bit_b = hash_beta(token);

    bf->storage_bytes[bit_a / 8] |= (1 << (bit_a % 8));
    bf->storage_bytes[bit_b / 8] |= (1 << (bit_b % 8));
}

bool check_token_presence(BloomFilter* bf, const char* token) {
    unsigned int bit_a = hash_alpha(token);
    unsigned int bit_b = hash_beta(token);

    bool match_a = bf->storage_bytes[bit_a / 8] & (1 << (bit_a % 8));
    bool match_b = bf->storage_bytes[bit_b / 8] & (1 << (bit_b % 8));
    return match_a && match_b;
}
