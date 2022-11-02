#ifndef STORAGE_H
#define STORAGE_H
#include <netinet/in.h>
#include <string>
using namespace std;

class Storage {
    private:
        int newSocket, addressLen;
        struct sockaddr_in address;

    public:
        Storage(string);
        void createSocket();
        void requestConnection();
        void closeConnection();
};
#endif