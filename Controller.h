#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <netinet/in.h>

class Controller {
    private:
        int new_socket, address_len, opt;
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