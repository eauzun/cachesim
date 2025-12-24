#include "CacheSimulator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;

CacheSimulator::CacheSimulator(const CacheConfig& cfg) : config(cfg) {
    //initializing statistics
    total_loads = 0;
    total_stores = 0;
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_misses = 0;
    compulsory_misses = 0;
    capacity_misses = 0;
    conflict_misses = 0;
    memory_reads = 0;
    memory_writes = 0;
    global_lru_counter = 0;
    global_fifo_counter = 0;

    //calculating cache parameters
    block_offset_bits = log2(config.block_size);

    if (config.associativity == 0) { //if fully associative
        num_sets = 1;
        config.associativity = config.cache_size / config.block_size;
    }
    else {
        num_sets = config.cache_size / (config.block_size * config.associativity);
    }
    
    index_bits = log2(num_sets);
    tag_bits = 32 - index_bits - block_offset_bits;

    //initializing cache
    cache.resize(num_sets);
    for (int i = 0; i < num_sets; i++) {
        cache[i].resize(config.associativity);
    }
}

void CacheSimulator::parseAddress(unsigned long address, unsigned long& tag, int& index) {
    unsigned long block_address = address >> block_offset_bits;
    index = block_address & ((1 << index_bits) - 1);
    tag = block_address >> index_bits;
}

int CacheSimulator::findBlock(int set_index, unsigned long tag) {
    for (int i = 0; i < config.associativity; i++) {
        if (cache[set_index][i].validBit && cache[set_index][i].tag == tag) {
            return i;
        }
    }
    return -1;
}

int CacheSimulator::findEmptyBlock(int set_index) {
    for (int i = 0; i < config.associativity; i++) {
        if (!cache[set_index][i].validBit) {
            return i;
        }
    }
    return -1;
}

int CacheSimulator::selectVictim(int set_index) {
    if (config.replacement_policy == "LRU") {
        int min_lru = cache[set_index][0].lru_counter;
        int victim = 0;
        for (int i = 1; i < config.associativity; i++) {
            if (cache[set_index][i].lru_counter < min_lru) {
                min_lru = cache[set_index][i].lru_counter;
                victim = i;
            }
        }
        return victim;
    }
    else if (config.replacement_policy == "FIFO") {
        int min_fifo = cache[set_index][0].fifo_order;
        int victim = 0;
        for (int i = 1; i < config.associativity; i++) {
            if (cache[set_index][i].fifo_order < min_fifo) {
                min_fifo = cache[set_index][i].fifo_order;
                victim = i;
            }
        }
        return victim;
    }
    else { //RANDOM
        return rand() % config.associativity;
    }
}

void CacheSimulator::classifyMiss(unsigned long address, int set_index) {
    unsigned long block_address = address >> block_offset_bits;

    //checking if compulsory miss
    if (ever_accessed.find(block_address) == ever_accessed.end()) {
        compulsory_misses++;
        ever_accessed[block_address] = true;
        return;
    }

    //check for capacity miss (cache is full and no invalid blocks)
    bool cache_full = true;
    for (int i = 0; i < num_sets; i++) {
        for (int j = 0; j < config.associativity; j++) {
            if (!cache[i][j].validBit) {
                cache_full = false;
                break;
            }
        }
        if (!cache_full) break;
    }

    //if the set is full but cache overall isn't full, it's a conflict miss
    bool set_full = (findEmptyBlock(set_index) == -1);

    if (set_full && !cache_full) 
        conflict_misses++;
    
    else if (cache_full) 
        capacity_misses++;
    
    else conflict_misses++;
    
}

void CacheSimulator::accessCache(char type, unsigned long address) {
    unsigned long tag;
    int set_index;
    parseAddress(address, tag, set_index);

    bool is_load = (type == 'R');

    if (is_load) 
        total_loads++;
    
    else 
        total_stores++;

    int block_index = findBlock(set_index, tag); //check for hit

    if (block_index != -1) {
        if (is_load)  //if hit
            load_hits++;
        else {
            store_hits++;
            if (config.write_policy == "WRITE_BACK") cache[set_index][block_index].dirtyBit = true;
            
            else memory_writes++; //if WRITE_THROUGH
        }
        if (config.replacement_policy == "LRU") cache[set_index][block_index].lru_counter = ++global_lru_counter; //updating LRU counter
    }
    else {//if miss
        if (is_load)
            load_misses++;
        else 
            store_misses++;

        classifyMiss(address, set_index);

        //handle write-allocate policy
        if (!is_load && !config.write_allocate){
            memory_writes++;
            return;
        }

        int target_block = findEmptyBlock(set_index); //finding a block to place data
        if (target_block == -1) {
            target_block = selectVictim(set_index);

            if (cache[set_index][target_block].dirtyBit) //if victim is dirty, write back
                memory_writes++;
        }

        memory_reads++; //loading block from memory

        //placing the new block
        cache[set_index][target_block].validBit = true;
        cache[set_index][target_block].tag = tag;
        cache[set_index][target_block].dirtyBit = false;
        cache[set_index][target_block].lru_counter = ++global_lru_counter;
        cache[set_index][target_block].fifo_order = ++global_fifo_counter;

        //handle store on miss
        if (!is_load) {
            if (config.write_policy == "WRITE_BACK") 
                cache[set_index][target_block].dirtyBit = true;
            else //"WRITE_THROUGH"
                memory_writes++;
            
        }
    }
}

void CacheSimulator::printStats() {
    long total_accesses = total_loads + total_stores;
    long total_hits = load_hits + store_hits;
    double hit_rate = (total_accesses > 0) ? (100.0 * total_hits / total_accesses) : 0.0;

    long total_misses = load_misses + store_misses;
    double miss_rate = (total_accesses > 0) ? ((double)total_misses / total_accesses) : 0.0;
    double amat = 1.0 + miss_rate * 100.0;

    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl;
    cout << "Load misses: " << load_misses << endl;
    cout << "Store hits: " << store_hits << endl;
    cout << "Store misses: " << store_misses << endl;
    cout << fixed << setprecision(2);
    cout << "Total hit rate: " << hit_rate << "%" << endl;
    cout << "Compulsory misses: " << compulsory_misses << endl;
    cout << "Capacity misses: " << capacity_misses << endl;
    cout << "Conflict misses: " << conflict_misses << endl;
    cout << "Memory reads: " << memory_reads << endl;
    cout << "Memory writes: " << memory_writes << endl;
    cout << "AMAT: " << amat << " cycles" << endl;
}

void processTrace(CacheSimulator& sim, const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open trace file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        char type;
        string addr_str;

        if (iss >> type >> addr_str) {
            unsigned long address = stoul(addr_str, nullptr, 16);
            sim.accessCache(type, address);
        }
    }

    file.close();
}
