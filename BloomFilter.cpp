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
    bits = new int[m];
}

/**
 * @brief Generates the bits assigned to 'entry'.
 * Utilized 'Enhanced Double Hashing' from P. Dillinger & P. Manolios
 * Reference: Bloom Filters in Probabilistic Verification
 * 
 * @param entry Value to be added to the bloom filter
 * @return int* Array of bits assigned to 'entry'
 */
void BloomFilter::getIndices(unsigned char* entry, int* hashes) {
    unsigned char buf256[32];
    unsigned char buf512[64];

    // Get SHA256 and SHA512 hashes
    SHA256(entry, strlen((const char *)entry), buf256);
    SHA512(entry, strlen((const char *)entry), buf512);

    // Collapse 256 and 512 hashes
    int x = 0, y = 0;
    for (int i = 0; i<32; i++) {
        x += buf256[i] % m;
        y += buf512[i] % m;
        y += buf512[i+32] % m;
    }
    x = x % m;
    y = y % m;

    // Enhanced Double Hashing
    hashes[0] = x;
    for (int i = 1; i<k; i++) {
        x = (x + y) % m;
        y = (i + y) % m;
        hashes[i] = x;
    }
}

/**
 * @brief Adds an entry into the bloom filter.
 * 
 * @param entry Value to add
 */
void BloomFilter::add(unsigned char* entry) {
    int* hashes = new int[k];
    getIndices(entry, hashes);
    for (int i = 0; i<k; i++) bits[hashes[i]] = 1;
    delete []hashes;
}

/**
 * @brief Query if the entry is found in the bloom filter.
 * 
 * @param entry Value to look up
 * @return true 
 * @return false 
 */
bool BloomFilter::query(unsigned char* entry) {
    bool found = true;
    int* hashes = new int[k];
    getIndices(entry, hashes);

    int i = 0;
    while (found && i < k) {
        if (!bits[hashes[i]]) found = false;
        i++;
    }
    delete []hashes;

    return found;
}

/**
 * @brief Prints bloom filter
 * 
 */
void BloomFilter::print() {
    std::cout << "[";
    for (int i = 0; i<m; i++) {
        std::cout << std::setw(2);
        if (bits[i]) std::cout << " " << i;
    }
    std::cout << " ]" << std::endl;
}

BloomFilter::~BloomFilter() {
    delete []bits;
}