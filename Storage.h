#ifndef STORAGE_H
#define STORAGE_H
#include <netinet/in.h>
#include <string>
using namespace std;

class Storage {
    private:
        // heartbeat and store/query sockets
        int hbSocket, sqSocket, hbLen, sqLen, opt;
        // heartbeat and store/query address
        struct sockaddr_in hbAddress, sqAddress;

    public:
        Storage();
        void beat();
        void createSocket();
        void requestConnection();
        void closeConnection();
        void sendBeat();
};
#endif