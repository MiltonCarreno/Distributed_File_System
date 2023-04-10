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

// Enum to represent state of storage nodes
enum NodeState {dead, alive};
// String array to print state of storage nodes
const std::string NodeStateStrings[] {"Dead", "Alive"};

/**
 * @file Controller.h
 * 
 * @brief Declaration of Controller Node class
 * 
 * @author Milton Carreno
 * 
 */
class Controller {
    private:
        std::mutex mapMutex; // Mutex to lock access to map
        // Records storage node info (i.e. port, space, lastbeat, state)
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
        void addFile(unsigned char*);
        bool lookUpFile(unsigned char*);
};
#endif