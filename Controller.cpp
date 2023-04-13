#include "Controller.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#define PORT 8080

using namespace std::chrono;

/**
 * @brief Constructs a new Controller object.
 * Sets up socket information.
 */
Controller::Controller() : bl(5000,4) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    addressLen = sizeof(address);
    opt = 1;
    printf("\nThis is the constructor\n");
}

/**
 * @brief Creates Controller socket.
 * 
 */
void Controller::createSocket() {
    // Creating socket file descriptor
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(newSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("\nCreated Socket\n");
}

/**
 * @brief Binds Controlle socket to PORT
 * 
 */
void Controller::bindSocket() {
    // Forcefully attaching socket to the port 8080
    if (bind(newSocket, (struct sockaddr*)&address, addressLen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("\nBound Socket\n");
}

/**
 * @brief Listens to for connections to PORT. 
 * 
 * @param queueLen Lenght of connection queue
 */
void Controller::listenConnection(int queueLen) {
    bool heard = (listen(newSocket, queueLen) == 0);
    if (!heard) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("\nHeard Connection\n");
}

/**
 * @brief Accepts connection and returns the connection
 * 
 * @return int Connection accepted
 */
int Controller::acceptConnection() {
    int connection = accept(newSocket, 
    (struct sockaddr*)&address, (socklen_t*)&addressLen);
    if (connection < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("\nAccepted Connection\n");
    return connection;
}

/**
 * @brief Shutsdown Controller socket
 * 
 */
void Controller::shutdownSocket() {
    shutdown(newSocket, SHUT_RDWR);
    printf("\nShutdown Listening Socket!\n");
}

/**
 * @brief Adds a node to map of nodes
 * 
 * @param nodePort Port of node to be added
 * @param nodeSpace Space of node to be added
 */
void Controller::addStorageNode(int nodePort, int nodeSpace) {
    // Obtain lock to prevent race conditions
    const std::lock_guard<std::mutex> lock(mapMutex);
    // Add storage node
    nodes[nodePort]["space"] = nodeSpace;
    nodes[nodePort]["state"] = NodeState::alive;
    nodes[nodePort]["lastBeat"] = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count();
    // Print node's info
    std::cout << "Port: " << nodePort << std::endl;
    std::cout << "Space: " << nodes[nodePort]["space"] << std::endl;
    std::cout << "Last Beat: " << nodes[nodePort]["lastBeat"] << std::endl;
}

/**
 * @brief Checks if nodes have sent a heartbeart in the last 10 seconds.
 * If not, the nodes are labeled 'DEAD' and no more data will be sent to them.
 */
void Controller::checkStorageNodes() {
    while (true) {
        {
            // Obtain lock to prevent race conditions
            const std::lock_guard<std::mutex> lock(mapMutex);
            std::cout << "\n@@@@@@@@@@@@@@@@@@@@@@@\n";
            // Iterate through storage nodes
            for (auto & node: nodes) {
                // Get current time
                int t_now = (duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()).count());
                std::cout << "+++++++++++++++++++++++" << std::endl; 
                // Print node's port and lastbeat
                std::cout << "Port: " << node.first << std::endl;
                std::cout << "Last Beat: " << nodes[node.first]["lastBeat"] << std::endl;
                std::cout << "State: ";
                // Calculate time since last beat and update node state accordingly
                if ((t_now - nodes[node.first]["lastBeat"]) > 10000) {
                    nodes[node.first]["state"] = NodeState::dead;
                } else {
                    nodes[node.first]["state"] = NodeState::alive;
                }
                std::cout << NodeStateStrings[nodes[node.first]["state"]] << std::endl;
                std::cout << "+++++++++++++++++++++++" << std::endl;
            }
            std::cout << "@@@@@@@@@@@@@@@@@@@@@@@\n";
        }
        // Wait for 10 seconds before checking nodes again
        std::this_thread::sleep_for(milliseconds(10000));
    };
}

/**
 * @brief Returns vector of alive nodes with at least 'fileSize' space available
 * 
 * @param fileSize Minimum amount of space needed
 * @return std::vector<int> Vector of alive nodes with sufficient space
 */
std::vector<int> Controller::getFreeStorageNodes(int fileSize) {
    // Obtain lock to prevent race conditions
    const std::lock_guard<std::mutex> lock(mapMutex);
    std::vector<int> availableNodes;
    // Iterate through storage nodes
    for (auto & node : nodes) {
        // Only node if is alive and has enough space available
        if (nodes[node.first]["state"] == NodeState::alive &&
        nodes[node.first]["space"] >= fileSize) {
            availableNodes.push_back(node.first);
        }
    }
    return availableNodes;
}

/**
 * @brief Produces the SHA256 of a given unsigned char*
 * 
 * @param s Name of file
 */
void Controller::addFile(unsigned char *s) {
    bl.print();
    bl.add(s);
    bl.print();
}

/**
 * @brief Searches for file in bloom filter
 * 
 * @param s Name of file
 * @return true 
 * @return false 
 */
bool Controller::lookUpFile(unsigned char *s) {
    return bl.query(s);
}