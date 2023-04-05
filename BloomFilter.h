#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

class BloomFilter {
    private:
        int m; // Length of bloomfilter (length is conventionally named 'm')
        int k; // Number of hashes
        int bits[]; // Array of 'on' or 'off' bits (i.e. bloomfilter)
    public:
        BloomFilter(int,int); // args (m,k)
        void add(unsigned char*);
        bool query(unsigned char*);
};

#endif