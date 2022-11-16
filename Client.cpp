#include "Client.h"
#include "Message.cpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#define PORT 9090
const char* LOCAL_HOST = "127.0.0.1";

Client::Client(int port, string file) {
    // Set file info
    fstream fs;
    fs.open(file, fstream::in | fstream::binary);
    if (fs.is_open()) {
        filePath = file;        // Save file path
        fs.seekg(0, fs.end);
        fileSize = fs.tellg();  // Get file size
        fs.seekg(0, fs.beg);
        fs.close();
    } else {
        cout << "File didn't open" << endl;
    }
    setSocket(port);
    printf("\nThis is the constructor\n");
}

void Client::setSocket(int port) {
    // Set socket info
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    addressLen = sizeof(address);

    if (inet_pton(AF_INET, LOCAL_HOST, &address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
    }
}

void Client::createSocket() {
    // Creating socket file descriptor
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("\nCreated Socket\n");
}

void Client::requestConnection() {
    int connection = connect(newSocket, 
    (struct sockaddr*)&address, (socklen_t)addressLen);
    if (connection < 0) {
        perror("Connection request error");
        exit(EXIT_FAILURE);
    }
    printf("\nConnection Request Accepted\n");
}

void Client::closeConnection() {
    int connection = close(newSocket);
    if (connection < 0) {
        exit(EXIT_FAILURE);
    }
}

void Client::sendFileInfo() {
    // Send file info to Controller
    MessageType msgType = store;
    FileInfo msg = {filePath, fileSize};
    send(newSocket, (const void*)&msgType, sizeof(msgType), 0);
    send(newSocket, (const void*)&msg, sizeof(msg), 0);
}

void Client::getStorageNodes() {
    // Get number of available storage nodes from Controller
    int numNodes = 0;
    read(newSocket, (void *)&numNodes, sizeof(numNodes));
    // Get one by one the storage nodes
    for (int i = 0; i < numNodes; i++) {
        int node;
        read(newSocket, (void *)&node, sizeof(node));
        nodes.push_back(node);
    }
}

void Client::sendChunks() {
    // Get file chunks
    fstream fs;
    fs.open(filePath, fstream::in | fstream::binary);

    // Connect to Storage nodes one by one
    for (int node: nodes) {
        setSocket(node);
        createSocket();
        requestConnection();
        // Send chunk
        byte chunk;
        send(newSocket, (void *)&chunk, sizeof(chunk), 0);
        closeConnection();
    }

}