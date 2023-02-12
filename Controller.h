#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <netinet/in.h>
#include <mutex>
#include <thread>
#include <map>
#include <vector>

class Controller {
    private:
        std::mutex mapMutex; // Mutex to lock access to map
        std::map<int,int> map; // Map to record storage node port and space
        int newSocket, addressLen, opt;
        struct sockaddr_in address;

    public:
        Controller();
        void createSocket();
        void bindSocket();
        void listenConnection(int);
        int acceptConnection();
        void shutdownSocket();
        void addStorageNode(int, int);
        std::vector<int>getFreeStorageNodes(int);
        void getHash(unsigned char*);
};
#endif