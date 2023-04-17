#include "Controller.h"
#include "Message.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <thread>

// Number of connection requests waiting to be accepted
const int MAX_NUM_CONN_REQS = 10;
// Number of total connections to be accepted
const int MAX_NUM_CONN = 13;

/**
 * @brief Handles message exchanges b/t Controller and Client/Storage nodes
 * 
 * @param serv 
 * @param connection 
 */
void chatFun(Controller *serv, int connection) {
    MessageType msgType;
    read(connection, (void *)&msgType, sizeof(msgType));
    std::cout << "\n******************************" << std::endl;
    std::cout << "-----------" << MessageTypeStrings[msgType] << "-----------" << std::endl;
    if (msgType == MessageType::store) {
        FileInfo file;
        read(connection, (void *)&file, sizeof(file));
        std::cout << "Name of file: " << file.fileName << std::endl;
        std::cout << "Size of file: " << file.size << std::endl;
        // Add file to bloom filter
        serv->addFile((unsigned char*)&file.fileName[0]);
        // Get storage nodes with free space
        std::vector<int> nodes = serv->getFreeStorageNodes(file.size);
        // Send storage node info one by one
        int numNodes = nodes.size();
        send(connection, (const void*)&numNodes, sizeof(numNodes), 0);
        for (int i = 0; i < numNodes; i++) {
            int node = nodes[i];
            send(connection, (const void*)&node, sizeof(node), 0);
        }
    } else if (msgType == MessageType::query) {
        FileInfo file;  // Get file info
        read(connection, (void *)&file, sizeof(file));
        // Check if file is in bloom filter
        std::cout << "Is file in the bloom filter? ";
        std::cout << serv->lookUpFile((unsigned char*)&file.fileName[0]) << std::endl;
    } else if (msgType == MessageType::heartbeat) {
        Heartbeat beat; // Get heartbeat
        read(connection, (void *)&beat, sizeof(beat));
        // Get storage node inventory size
        int invSize;
        read(connection, (void *)&invSize, sizeof(invSize));
        // Get storage node inventory
        std::vector<std::string> inv;
        for (int i = 0; i < invSize; i++) {
            std::string chunkName;
            read(connection, (void *)&chunkName, sizeof(chunkName));
            inv.push_back(chunkName);
        }
        // Record storage node and its inventory
        serv->addStorageNode(beat.port, beat.space, inv);
    }
    std::cout << "******************************" << std::endl;
    // Close connection socket
    close(connection);
}

/**
 * @brief Main function that initiallizes a Controller object
 * 
 * @param argc No arguments
 * @param argv No arguments
 * @return int 
 */
int main(int argc, char *argv[]) {
    // Create Controller
    Controller server;
    // Create and bind new listening socket
    server.createSocket();
    server.bindSocket();
    // Accept connection request and create new connection socket
    std::vector<std::thread> threads;
    threads.emplace_back(std::thread(&Controller::checkStorageNodes, &server));
    for (int i = 0; i<MAX_NUM_CONN; i++) {
        server.listenConnection(MAX_NUM_CONN_REQS);
        int conn = server.acceptConnection();
        // Delegate chatting/connections to individual threads
        threads.emplace_back(std::thread(chatFun, &server, conn));
    }
    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }
    // Close listening socket
    server.shutdownSocket();
    return 0;
}