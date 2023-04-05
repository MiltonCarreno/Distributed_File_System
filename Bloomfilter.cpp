#include "Bloomfilter.h"

/**
 * @brief Construct a new Bloomfilter:: Bloomfilter object.
 * 
 * @param size 
 * @param numHashes 
 */
Bloomfilter::Bloomfilter(int size, int numHashes) {
    m = size;
    k = numHashes;
}

/**
 * @brief Adds an entry into the bloomfilter.
 * 
 * @param entry Value to add
 */
void Bloomfilter::addEntry(unsigned char* entry) {

}

/**
 * @brief Looks up if the entry is found in the bloomfilter.
 * 
 * @param entry Value to look up
 * @return true 
 * @return false 
 */
bool Bloomfilter::lookUp(unsigned char* entry) {

}