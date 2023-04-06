#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

class BloomFilter {
    private:
        int m; // Length of bloomfilter (length is conventionally named 'm')
        int k; // Number of hashes
        int *bits; // Array of 'on'/'off' bits (i.e. bloomfilter)
    public:
        BloomFilter(int,int); // Args (m,k)
        void getIndices(unsigned char*,int*); // Get bits: to turn 'on' or to check if 'on'
        void add(unsigned char*); // Add entry into bloom filter
        bool query(unsigned char*); // Determine if entry is in bloom filter
        void print(); // Prints bloom filter to terminal
        ~BloomFilter(); // Destructor to delete alloc mem for 'bits'
};

#endif