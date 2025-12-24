#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "CacheConfig.h"
#include "CacheBlock.h"
#include <vector>
#include <map>
#include <string>
#include <vector>
using namespace std;

class CacheSimulator {
private:
    CacheConfig config;
    vector<vector<CacheBlock>> cache;

    //cache parameters
    int num_sets;
    int block_offset_bits;
    int index_bits;
    int tag_bits;

    //statistics
    long total_loads;
    long total_stores;
    long load_hits;
    long load_misses;
    long store_hits;
    long store_misses;
    long compulsory_misses;
    long capacity_misses;
    long conflict_misses;
    long memory_reads;
    long memory_writes;

    //replacement policy counters
    int global_lru_counter;
    int global_fifo_counter;

    map<unsigned long, bool> ever_accessed; //for capacity miss detection

    //Private helper methods
    void parseAddress(unsigned long address, unsigned long& tag, int& index);
    int findBlock(int set_index, unsigned long tag);
    int findEmptyBlock(int set_index);
    int selectVictim(int set_index);
    void classifyMiss(unsigned long address, int set_index);

public:
    CacheSimulator(const CacheConfig& cfg); //Constructor

    void accessCache(char type, unsigned long address); //main simulation method

    void printStats();
};

void processTrace(CacheSimulator& sim, const string& filename);

#endif
