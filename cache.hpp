#ifndef CACHE_HPP
#define CACHE_HPP
#include <string>

#include <vector>
#include <deque>

struct CacheBlock {
    bool valid;
    unsigned int tag;
};

class Cache {
public:
    Cache(int cacheSize, int blockSize, int associativity);
    bool access(unsigned int address, int globalTime); // returns true if hit
    void printStats(const std::string& label) const;


    // ✅ Move these to public so main.cpp can access them
    int getHits() const { return hits; }
    int getMisses() const { return misses; }

private:
    int cacheSize;
    int blockSize;
    int associativity;
    int numSets;
    int indexBits, offsetBits;

    std::vector<std::deque<CacheBlock>> sets;

    int hits = 0;
    int misses = 0;

    unsigned int getIndex(unsigned int address) const;
    unsigned int getTag(unsigned int address) const;
};

#endif
