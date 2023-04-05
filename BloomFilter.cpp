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
 * @brief Generates the bits assigned to 'entry'
 * 
 * @param entry Value to be added to the bloom filter
 * @return int* Array of bits assigned to 'entry'
 */
int* BloomFilter::getIndices(unsigned char* entry) {
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

    // Execute 'Enhanced Double Hashing'
    // Reference: Bloom Filters in Probabilistic Verification
    hashes[x] = 1; // Set bit on
    for (int i = 1; i<k; i++) {
        x = (x + y) % m;
        y = (i + y) % m;
        hashes[x] = 1; // Set bit on
    }

    return hashes;
}

/**
 * @brief Adds an entry into the bloom filter.
 * 
 * @param entry Value to add
 */
void BloomFilter::add(unsigned char* entry) {
    int hashes[k] = getIndices(entry);
    for (auto idx: hashes) bits[idx] = 1;
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
    int hashes[k] = getIndices(entry), i = 0;

    while (found == true && i < k) {
        if (!bits[hashes[i]]) found = false;
    }

    return found;
}