#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "cache.hpp"
using namespace std;

struct MemoryAccess {
    char operation;         // 'R' or 'W'
    unsigned int address;   // parsed hex to int
};

int main(int argc, char* argv[]) 
    {
        if (argc < 7) 
        {
            cerr << "Usage: " << argv[0] << " <trace_file> <L1_size> <L1_assoc> <L2_size> <L2_assoc> <block_size>\n";
            return 1;
        }

    string traceFile = argv[1];
    int L1_SIZE, L1_ASSOC, L2_SIZE, L2_ASSOC, BLOCK_SIZE;

    try {
        L1_SIZE = std::stoi(argv[2]);
        L1_ASSOC = std::stoi(argv[3]);
        L2_SIZE = std::stoi(argv[4]);
        L2_ASSOC = std::stoi(argv[5]);
        BLOCK_SIZE = std::stoi(argv[6]);
    } catch (std::exception& e) {
        std::cerr << "Error: All cache parameters must be valid integers.\n";
        return 1;
    }

    if (L1_SIZE <= 0 || L1_ASSOC <= 0 || L2_SIZE <= 0 || L2_ASSOC <= 0 || BLOCK_SIZE <= 0) {
        std::cerr << "Error: All sizes and associativity must be positive non-zero values.\n";
        return 1;
    }

    int l1_blocks = L1_SIZE / BLOCK_SIZE;
    int l2_blocks = L2_SIZE / BLOCK_SIZE;

    if (L1_ASSOC > l1_blocks) {
        std::cerr << "Error: L1 associativity exceeds number of blocks in L1.\n";
        return 1;
    }

    if (L2_ASSOC > l2_blocks) {
        std::cerr << "Error: L2 associativity exceeds number of blocks in L2.\n";
        return 1;
    }

ifstream infile(traceFile);

    string line;
    vector<MemoryAccess> accesses;

    if (!infile.is_open()) {
        cerr << "Failed to open trace.txt" << endl;
        return 1;
    }

    while (getline(infile, line)) {
        istringstream iss(line);
        char op;
        string hexAddr;
        unsigned int addr;

        if (!(iss >> op >> hexAddr)) {
            cerr << "Skipping invalid line: " << line << endl;
            continue;
        }

        addr = stoul(hexAddr, nullptr, 16);
        accesses.push_back({op, addr});
    }
    infile.close();

    Cache l1(L1_SIZE, BLOCK_SIZE, L1_ASSOC);
    Cache l2(L2_SIZE, BLOCK_SIZE, L2_ASSOC);


    int time = 0;
    for (const auto& access : accesses) {
        bool l1_hit = l1.access(access.address, time++);
        
        if (l1_hit) {
            cout << "Access: " << access.operation
                    << " 0x" << hex << uppercase << access.address
                    << " -> L1 Hit" << endl;
        } else {
            bool l2_hit = l2.access(access.address, time++);
            
            if (l2_hit) {
                // simulate refill from L2 to L1
                l1.access(access.address, time++);
                cout << "Access: " << access.operation
                        << " 0x" << hex << uppercase << access.address
                        << " -> L1 Miss -> L2 Hit -> Loaded to L1" << endl;
            } else {
                cout << "Access: " << access.operation
                        << " 0x" << hex << uppercase << access.address
                        << " -> L1 Miss -> L2 Miss -> Fetched from Memory" << endl;
            }
        }
    }

    l1.printStats("L1");
    cout << "\nL2 Cache Stats:\n";
    l2.printStats("L2");

    // Constants for timing (in CPU cycles)
const int L1_HIT_TIME = 1;
const int L2_HIT_TIME = 10;
const int MEM_TIME = 50;

// Totals
int l1Hits = l1.getHits();
int l1Misses = l1.getMisses();
int l2Hits = l2.getHits();
int l2Misses = l2.getMisses();

int totalAccesses = l1Hits + l1Misses;

double l1MissRate = (totalAccesses > 0) ? ((double)l1Misses / totalAccesses) : 0.0;
double l2MissRate = (l1Misses > 0) ? ((double)l2Misses / l1Misses) : 0.0;

double amat = L1_HIT_TIME + 
              l1MissRate * (L2_HIT_TIME + l2MissRate * MEM_TIME);

cout << "\nPerformance Summary:\n";
cout << "AMAT = " << L1_HIT_TIME << " + " 
     << l1MissRate << " x (" << L2_HIT_TIME << " + " 
     << l2MissRate << " x " << MEM_TIME << ")"
     << " = " << amat << " cycles\n";


    return 0;
}
