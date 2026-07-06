#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

typedef struct BloomFilter BloomFilter;
BloomFilter* init_bloom();
void register_token(BloomFilter* bf, const char* token);
bool check_token_presence(BloomFilter* bf, const char* token);

#endif
