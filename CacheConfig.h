#ifndef CACHE_CONFIG_H
#define CACHE_CONFIG_H

#include <string>

struct CacheConfig {
    int cache_size;
    int block_size;
    int associativity;
    std::string replacement_policy;
    std::string write_policy;
    bool write_allocate;
};

CacheConfig parseConfig(const std::string& filename);

#endif
