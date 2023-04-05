#include "BloomFilter.h"
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>

/**
 * @brief Construct a new BloomFilter object.
 * 
 * @param size 
 * @param numHashes 
 */
BloomFilter::BloomFilter(int size, int numHashes) {
    m = size;
    k = numHashes;
}

/**
 * @brief Adds an entry into the bloom filter.
 * 
 * @param entry Value to add
 */
void BloomFilter::add(unsigned char* entry) {
    unsigned char buf256[32];
    unsigned char buf512[64];

    // Get SHA256 and SHA512 hashes
    SHA256(entry, strlen((const char *)entry), buf256);
    SHA512(entry, strlen((const char *)entry), buf512);

    std::cout << std::hex // hex
         << std::internal // fill the number
         << std::setfill('0'); // fill with 0s
    
    for (int i = 0; i<32; i++) {
        int x = buf256[i];
        std::cout << std::setw(2) << x;
    }
    std::cout << std::endl;
    for (int i = 0; i<64; i++) {
        int x = buf512[i];
        std::cout << std::setw(2) << x;
    }

    std::cout << std::endl;
    std::cout << std::dec << std::endl;

    // Compress 256 and 512 hashes into ints
    int x = 0;
    int y = 0;
    for (int i = 0; i<32; i++) {
        x += buf256[i] % m;
        y += buf512[i] % m;
        y += buf512[i+32] % m;
    }
    x = x % m;
    y = y % m;

    // Execute 'Enhanced Double Hashing'
    // Reference: Bloom Filters in Probabilistic Verification
    int hashes[k];
    hashes[0] = x;
    for (int i = 1; i<k; i++) {
        x = (x + y) % m;
        y = (i + y) % m;
        hashes[i] = x;
    }

}

/**
 * @brief Query if the entry is found in the bloom filter.
 * 
 * @param entry Value to look up
 * @return true 
 * @return false 
 */
bool BloomFilter::query(unsigned char* entry) {

}