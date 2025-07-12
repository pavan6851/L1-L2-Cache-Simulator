#include "cache.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>  // For setprecision

using namespace std;

Cache::Cache(int cacheSize, int blockSize, int associativity)
    : cacheSize(cacheSize), blockSize(blockSize), associativity(associativity) {
    
    numSets = cacheSize / (blockSize * associativity);
    sets.resize(numSets);

    offsetBits = log2(blockSize);
    indexBits = log2(numSets);
}

unsigned int Cache::getIndex(unsigned int address) const {
    return (address >> offsetBits) & ((1 << indexBits) - 1);
}

unsigned int Cache::getTag(unsigned int address) const {
    return address >> (offsetBits + indexBits);
}

bool Cache::access(unsigned int address, int globalTime) {
    unsigned int index = getIndex(address);
    unsigned int tag = getTag(address);

    // Search the set for a matching tag
    for (auto& block : sets[index]) {
        if (block.valid && block.tag == tag) {
            hits++;
            return true; // Cache HIT
        }
    }

    // MISS - Apply FIFO replacement if needed
    misses++;

    if (sets[index].size() >= static_cast<size_t>(associativity)) {
        sets[index].pop_front(); // Remove oldest block (FIFO)
    }

    sets[index].push_back({true, tag}); // Insert new block
    return false;
}

void Cache::printStats(const std::string& label) const {
    int total = hits + misses;
    double hitRate = (total > 0) ? (static_cast<double>(hits) / total * 100.0) : 0.0;

    cout << "\n" << label << " Cache Stats:\n";
    cout << "Total Accesses: " << total << "\n";
    cout << "Hits: " << hits << ", Misses: " << misses << "\n";
    cout << "Hit Rate: " << fixed << setprecision(2) << hitRate << "%\n";
}
