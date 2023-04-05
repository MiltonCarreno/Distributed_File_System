#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

class Bloomfilter {
    private:
        int m; // Length of bloomfilter (length is conventionally named 'm')
        int k; // Number of hashes
        int bl[]; // Array of 'on' or 'off' bits (i.e. bloomfilter)
    public:
        Bloomfilter(int,int); // args (m,k)
        void addEntry(unsigned char*);
        bool lookUp(unsigned char*);
};

#endif