**L1/L2 Cache Simulator (C++)**

A configurable, high-performance CPU cache simulator implemented in C++.
This project models a two-level cache system (L1 and L2), using set-associative mapping with a FIFO replacement policy, and calculates the Average Memory Access Time (AMAT) for a given trace of memory accesses.

**Features**

    Configurable cache parameters (size, associativity, block size).
    Set-associative L1 and L2 caches.
    FIFO replacement policy.
    Cache hit/miss tracking.
    AMAT (Average Memory Access Time) calculation.
    Reads memory traces from a hex-formatted file.


**File Structure**

| File        | Description                                                 |
| ----------- | ----------------------------------------------------------- |
| `main.cpp`  | Main driver code to parse input, simulate, and output stats |
| `cache.cpp` | Implementation of the Cache class logic                     |
| `cache.hpp` | Header file defining the Cache class                        |
| `trace.txt` | Sample input trace file (hex accesses)                      |
| `README.md` | Project documentation                                       |


**Compilation**
g++ main.cpp cache.cpp -o cache_sim

**Usage**
./cache_sim <trace_file> <L1_size> <L1_assoc> <L2_size> <L2_assoc> <block_size>

**Example**
./cache_sim trace.txt 32768 4 262144 8 64

  trace.txt: File with memory access traces
  32768: L1 cache size in bytes (32KB)
  4: L1 cache associativity
  262144: L2 cache size in bytes (256KB)
  8: L2 cache associativity
  64: Block size in bytes


**Sample Output**

  Access: R 0xA1B2 -> L1 Miss -> L2 Miss -> Fetched from Memory
  Access: W 0xA1B4 -> L1 Hit
  Access: R 0xA1C0 -> L1 Miss -> L2 Miss -> Fetched from Memory
  Access: R 0xA1B2 -> L1 Hit
  Access: W 0xB123 -> L1 Miss -> L2 Miss -> Fetched from Memory
  
  L1 Cache Stats:
  Total Accesses: 5
  Hits: 2, Misses: 3
  Hit Rate: 40.00%
  
  L2 Cache Stats:
  
  L2 Cache Stats:
  Total Accesses: 3
  Hits: 0, Misses: 3
  Hit Rate: 0.00%
  
  Performance Summary:
  AMAT = 1 + 0.60 x (A + 1.00 x 32) = 37.00 cycles


