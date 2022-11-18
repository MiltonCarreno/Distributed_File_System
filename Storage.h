#ifndef STORAGE_H
#define STORAGE_H
#include <netinet/in.h>
#include <string>

class Storage {
    private:
        // Storage path
        std::string path;
        // Port and space currently available
        int port, space;
        // heartbeat and store/query sockets
        int hbSocket, sqSocket, hbLen, sqLen, opt;
        // heartbeat and store/query address
        struct sockaddr_in hbAddress, sqAddress;

    public:
        Storage(int, std::string);
        void createSocket();
        void requestConnection();
        void closeConnection();
        void bindSocket();
        void listenConnection(int);
        int acceptConnection();
        void sendBeat();
};
#endif