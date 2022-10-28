#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>

class Client {
    private:
        int newSocket, addressLen;
        struct sockaddr_in address;

    public:
        Client();
        int createSocket();
        void requestConnection();
        void closeConnection();
};
#endif