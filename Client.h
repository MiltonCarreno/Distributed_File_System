#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>

class Client {
    private:
        int newSocket, addressLen;
        struct sockaddr_in address;

    public:
        Client();
        void createSocket();
        int requestConnection();
        void closeConnection(int);
};
#endif