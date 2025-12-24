#include "CacheBlock.h"

CacheBlock::CacheBlock() : validBit(false), dirtyBit(false), tag(0), lru_counter(0), fifo_order(0) {}
