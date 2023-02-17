#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <netinet/in.h>
#include <mutex>
#include <chrono>
#include <thread>
#include <map>
#include <vector>

using namespace std::chrono;

class Controller {
    private:
        std::mutex mapMutex; // Mutex to lock access to map
        std::map<int,int> map; // Map to record storage node port and space
        // Map for storage node port and node state (i.e. alive or death)
        std::map<int, int> aliveNodes;
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
        void checkStorageNodes();
        std::vector<int>getFreeStorageNodes(int);
        void getHash(unsigned char*);
};
#endif