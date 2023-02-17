#include "Controller.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#define PORT 8080

Controller::Controller() {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    addressLen = sizeof(address);
    opt = 1;
    printf("\nThis is the constructor\n");
}

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

void Controller::bindSocket() {
    // Forcefully attaching socket to the port 8080
    if (bind(newSocket, (struct sockaddr*)&address, addressLen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("\nBound Socket\n");
}

void Controller::listenConnection(int queueLen) {
    bool heard = (listen(newSocket, queueLen) == 0);
    if (!heard) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("\nHeard Connection\n");
}

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

void Controller::shutdownSocket() {
    shutdown(newSocket, SHUT_RDWR);
    printf("\nShutdown Listening Socket!\n");
}

void Controller::addStorageNode(int nodePort, int nodeSpace) {
    const std::lock_guard<std::mutex> lock(mapMutex);
    map[nodePort] = nodeSpace;
    aliveNodes[nodePort] = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count();

    for (const auto& [key, val] : map) {
        std::cout << '[' << key << "] = " << val;
        if (aliveNodes[key] != -1) {
            std::cout << " T: " << aliveNodes[key] << "; ";
        } else {
            std::cout << " T: DEAD; ";
        }
    }
    std::cout << '\n';
}

void Controller::checkStorageNodes() {
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(mapMutex);
            std::cout << "\n@@@@@@@@@@@@@@@@@@@@@@@\n";
            for (auto [node, ms]: aliveNodes) {
                int t_now = (duration_cast<milliseconds>(
                    system_clock::now().time_since_epoch()).count());
                if ((t_now - ms) > 10000) aliveNodes[node] = -1;

                std::cout << '[' << node << "] <=> ";
                if (aliveNodes[node] != -1) {
                    std::cout << " T: " << aliveNodes[node] << "; ";
                } else {
                    std::cout << " T: DEAD; ";
                }
            }
            std::cout << "\n@@@@@@@@@@@@@@@@@@@@@@@\n";
        }
        std::this_thread::sleep_for(milliseconds(15000));
    };
}

std::vector<int> Controller::getFreeStorageNodes(int fileSize) {
    const std::lock_guard<std::mutex> lock(mapMutex);
    std::vector<int> availableNodes;
    for (const auto& [key, val]: map) {
        if (aliveNodes[key] != -1) availableNodes.push_back(key);
    }
    return availableNodes;
}

void Controller::getHash(unsigned char *s) {
    unsigned char obuf[32];

    SHA256(s, strlen((const char *)s), obuf);

    std::cout << std::hex // hex
         << std::internal // fill the number
         << std::setfill('0'); // fill with 0s
    
    for (int i = 0; i<32; i++) {
        int x = obuf[i];
        std::cout << std::setw(2) << x;
    }
    std::cout << std::dec << std::endl;
}