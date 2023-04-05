#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

class BloomFilter {
    private:
        int m; // Length of bloomfilter (length is conventionally named 'm')
        int k; // Number of hashes
        int bits[k] = {0}; // Array of 'on' or 'off' bits (i.e. bloomfilter)
    public:
        BloomFilter(int,int); // Args (m,k)
        int* getIndices(unsigned char*); // Get bits: to turn 'on' or to check if 'on'
        void add(unsigned char*);
        bool query(unsigned char*);
};

#endif