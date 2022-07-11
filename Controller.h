#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <netinet/in.h>

class Controller {
    private:
        int newSocket, addressLen, opt;
        struct sockaddr_in address;

    public:
        Controller();
        void createSocket();
        void bindSocket();
        bool listenConnection(int);
        int acceptConnection();
        void shutdownSocket();
};
#endif