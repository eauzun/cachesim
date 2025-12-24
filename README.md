# Advanced-Cache-Simulator

A configurable cache simulator implementation in C++ for modeling CPU cache behavior and analyzing performance metrics.
## 📋 Project Overview
This cache simulator models different cache architectures and measures their performance by processing memory trace files. It supports various configurations including different associativity levels, replacement policies, and write strategies.
## ✨ Features

### Cache Organizations

  •  Direct-mapped cache
      
  •  N-way set-associative cache
      
  •  Fully associative cache


### Replacement Policies

  •  LRU (Least Recently Used)
  
  •  FIFO (First In First Out)
  
  •  Random


### Write Policies

  •  Write-back with dirty bit tracking
  
  •  Write-through


### Write Allocation

  •  Write-allocate
  
  •  No-write-allocate


### Performance Metrics

  •  Hit/Miss statistics (separate for loads and stores)
  
  •  Miss classification (compulsory, capacity, conflict)
  
  •  Memory traffic analysis (reads and writes)
  
  •  AMAT (Average Memory Access Time) calculation



## 🛠️ Technologies
  •  Language: C++17
  
  •  Libraries: C++ Standard Library only
  
  •  Build System: Make / g++

📁 Project Structure

    .
    ├── CacheConfig.h           # Configuration structure declaration
    ├── CacheConfig.cpp         # Configuration file parser
    ├── CacheBlock.h            # Cache block structure declaration
    ├── CacheBlock.cpp          # Cache block implementation
    ├── CacheSimulator.h        # Main simulator class declaration
    ├── CacheSimulator.cpp      # Simulator implementation
    ├── cache_sim.cpp           # Program entry point, as main file
    ├── README.md               # This file
    ├── config.txt              # Configuration file
    └── trace.txt               # Memory Access Trace
## 🚀 Getting Started
### Prerequisites

  •  C++ compiler with C++17 support (g++, clang++, or MSVC)
  
  •  Make (optional, for using Makefile)

### Compilation

Manual compilation:

    g++ -std=c++17 -O2 CacheConfig.h CacheConfig.cpp CacheBlock.h CacheBlock.cpp CacheSimulator.h CacheSimulator.cpp cache_sim.cpp -o cache_sim

Running the Simulator

    ./cache_sim config.txt trace.txt

## 📝 Configuration File Format
Create a configuration file (e.g., config.txt) with the following format:

    CACHE_SIZE=4096
    BLOCK_SIZE=16
    ASSOCIATIVITY=4
    REPLACEMENT_POLICY=LRU
    WRITE_POLICY=WRITE_BACK
    WRITE_ALLOCATE=1
## Configuration Parameters

|Parameter | Description | Valid Values|
|--------- | ----------- | ------------|
|CACHE_SIZE | Total cache size in bytes | Power of 2 (e.g., 1024, 4096)|
|BLOCK_SIZE | Block/line size in bytes | Power of 2 (e.g., 16, 32, 64)|
|ASSOCIATIVITY | Cache associativity | 1 (direct-mapped), N (N-way), 0 (fully associative)|
|REPLACEMENT_POLICY |Replacement algorithm | LRU, FIFO, RANDOM|
|WRITE_POLICY | Write strategy | WRITE_BACK, WRITE_THROUGH|
|WRITE_ALLOCATE | Write allocation policy | 1 (write-allocate), 0 (no-write-allocate)|
## 📊 Trace File Format
Create a trace file (e.g., trace.txt) with memory access patterns:

    R 0x0000
    W 0x00A4
    R 0x00A8
    W 0x1000
 Format:

    R = Read operation
    W = Write operation
    Address in hexadecimal

## 📤 Output Format

      Total loads: X
      Total stores: Y
      Load hits: X
      Load misses: Y
      Store hits: X
      Store misses: Y
      Total hit rate: X.XX%
      Compulsory misses: X
      Capacity misses: X
      Conflict misses: X
      Memory reads: X
      Memory writes: X
      AMAT: X.XX cycles

## 🔧 Implementation Details
### Data Structures

### CacheBlock: Represents a single cache line

  •  Valid bit
  
  •  Dirty bit (for write-back)
  
  •  Tag field
  
  •  LRU counter
  
  •  FIFO order


### CacheSimulator: Main simulation engine

  •  2D vector for cache storage (sets × ways)
  
  •  Statistics counters
  
  •  Address parsing logic
  
  •  Replacement policy algorithms



### Miss Classification Algorithm

  •  Compulsory Miss: First access to a memory block
  
  •  Capacity Miss: Cache is full, block was previously accessed
  
  •  Conflict Miss: Set is full but cache has empty blocks elsewhere

## AMAT Calculation
    AMAT = Hit Time + Miss Rate × Miss Penalty
    AMAT = 1 + (Total Misses / Total Accesses) × 100 cycles
## 📈 Performance Analysis
The simulator can be used to analyze:

  •  Effect of associativity on hit rate
  
  •  Comparison of replacement policies (LRU vs FIFO vs Random)
  
  •  Impact of write policies on memory traffic
  
  •  Cache size vs performance trade-offs
  
  •  Miss distribution (compulsory vs capacity vs conflict)
