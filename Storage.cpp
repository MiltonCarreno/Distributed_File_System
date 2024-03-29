#include "Storage.h"
#include "Message.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <regex>
#define PORT_ONE 8080
#define PORT_TWO 9090
const char* LOCAL_HOST = "127.0.0.1";

Storage::Storage(int storagePort, std::string storagePath) {
    // Set port, space (2 MB) and path for storage node
    port = storagePort;
    space = 2000000;
    path = storagePath;
    // Set heartbeat socket
    hbAddress.sin_family = AF_INET;
    hbAddress.sin_port = htons(PORT_ONE);
    hbLen = sizeof(hbAddress);

    if (inet_pton(AF_INET, LOCAL_HOST, &hbAddress.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
    }

    // Set store/query socket
    sqAddress.sin_family = AF_INET;
    sqAddress.sin_addr.s_addr = INADDR_ANY;
    sqAddress.sin_port = htons(storagePort);
    sqLen = sizeof(sqAddress);
    opt = 1;

    printf("\nThis is the constructor\n");
}

void Storage::createSocket() {
    // Creating store/query socket
    if ((sqSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Store/Query Socket Failed");
        exit(EXIT_FAILURE);
    }
    // Forcefully attaching store/query socket to port 9090
    if (setsockopt(sqSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("\nCreated Sockets\n");
}

void Storage::createHeartBeatSocket() {
    // Creating heartbeat socket
    if ((hbSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("HeartBeat Socket Failed");
        exit(EXIT_FAILURE);
    }
    printf("Created Heartbeat Socket\n");
}

void Storage::requestHeartBeatConnection() {
    int connection = connect(hbSocket, 
    (struct sockaddr*)&hbAddress, (socklen_t)hbLen);
    if (connection < 0) {
        perror("Connection request error");
        exit(EXIT_FAILURE);
    }
    printf("\nHeartBeat Connection Accepted\n");
}

void Storage::closeHeartBeatSocket() {
    int connection = close(hbSocket);
    if (connection < 0) {
        exit(EXIT_FAILURE);
    }
    printf("\nHeartbeat Socket Closed\n");
}

void Storage::closeConnection() {
    int connection = close(hbSocket);
    if (connection < 0) {
        exit(EXIT_FAILURE);
    }
    connection = close(sqSocket);
    if (connection < 0) {
        exit(EXIT_FAILURE);
    }
}

void Storage::bindSocket() {
    // Forcefully attaching socket to the port 8080
    if (bind(sqSocket, (struct sockaddr*)&sqAddress, sqLen) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("\nBound Socket\n");
}

void Storage::listenConnection(int queueLen) {
    bool heard = (listen(sqSocket, queueLen) == 0);
    if (!heard) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("\nHeard Connection\n");
}

int Storage::acceptConnection() {
    int connection = accept(sqSocket, 
    (struct sockaddr*)&sqAddress, (socklen_t*)&sqLen);
    if (connection < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("\nAccepted Connection\n");
    return connection;
}

void Storage::sendBeat() {
    Heartbeat hb = {path, port, space};
    MessageType msgType = heartbeat;
    send(hbSocket, (const void*)&msgType, sizeof(msgType), 0);
    send(hbSocket, (const void*)&hb, sizeof(hb), 0);

    std::cout << "\n********HEARTBEAT********" << std::endl;
    printInventory();
    std::cout << "^^^^^^^^Inventory^^^^^^^^\n" << std::endl;

    int invSize = inv.size();
    std::cout << "Size:" << invSize << std::endl;
    send(hbSocket, (const void*)&invSize, sizeof(invSize), 0);
    for (auto i: inv) {
        send(hbSocket, (const void*)&i.first, sizeof(i.first), 0);
    }
}

void Storage::saveChunkFile(char *chunk, std::string fileName, std::string chunkName, int chunkSize) {
    // Write chunk file to path
    std::fstream fs(path + "/" + chunkName,
        std::fstream::out | std::fstream::binary | std::fstream::trunc);
    fs.write(chunk, chunkSize);
    fs.close();
    // Update space of Storage node
    space -= chunkSize;
    // Add chunk to inventory
    addChunkToInventory(fileName, chunkName);
    std::cout << "\n********ADD-CHUNK********" << std::endl;
    printInventory();
    std::cout << "^^^^^^^^Inventory^^^^^^^^\n" << std::endl;
}

void Storage::addChunkToInventory(std::string fileName, std::string chunkName) {
    const std::lock_guard<std::mutex> lock(invMutex);
    inventory.push_back(chunkName);  // Add chunk file name to vector

    inv[fileName].push_back(chunkName);
    // Create 'Inventory' if it doesn't exits
    std::ofstream invFile(path + "/Inventory.txt",
        std::ofstream::out | std::ofstream::app);
    invFile << fileName << std::endl;    // Add chunk file name to 'Inventory' file
    invFile.close();
}

void Storage::printInventory() {
    const std::lock_guard<std::mutex> lock(invMutex);
    if (inventory.size() > 0) {
        std::cout << "INVENTORY: ";
        for (auto i : inventory) std::cout << "[" << i << "]";
        std::cout << std::endl;
        for (auto i : inv) {
            std::cout << i.first << std::endl;
            for (auto j : i.second) std::cout << "\t " << j << std::endl;
        }
    } else {
        std::cout << "--EMPTY INVENTORY--";
    }
    std::cout << std::endl;
}