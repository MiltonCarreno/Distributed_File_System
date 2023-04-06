#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "BloomFilter.h"
#include <netinet/in.h>
#include <mutex>
#include <chrono>
#include <thread>
#include <map>
#include <vector>
#include <string>

using namespace std::chrono;

enum NodeState {dead, alive};

class Controller {
    private:
        std::mutex mapMutex; // Mutex to lock access to map
        // Records storage node info (i.e. port, space, state)
        std::map<int, std::map<std::string, int>> nodes;
        int newSocket, addressLen, opt;
        struct sockaddr_in address;
        BloomFilter bl;

    public:
        Controller();
        void createSocket();
        void bindSocket();
        void listenConnection(int);
        int acceptConnection();
        void shutdownSocket();
        void addStorageNode(int, int);
        void checkStorageNodes();
        std::vector<int>getFreeStorageNodes(int);
        void getHash(unsigned char*);
};
#endif