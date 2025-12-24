#ifndef CACHE_BLOCK_H
#define CACHE_BLOCK_H

struct CacheBlock { //represents a single cache line
    bool validBit;
    bool dirtyBit; //for write-back
    unsigned long tag; //tag bits, cant be negative
    int lru_counter; //counter for LRU replacement
    int fifo_order; //order for FIFO replacement

    CacheBlock(); //Constructor
};

#endif
