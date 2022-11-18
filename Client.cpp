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
#include <thread>
#include <regex>
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

string Client::parseFilePath(int num) {
    smatch m1, m2;
    std::regex e1("[^/]+\\w+$"), e2("\\.");
    std::regex_search(filePath, m1, e1);
    string chunkName = m1.str();

    if (std::regex_search(chunkName, m2, e2)) {
        chunkName = m2.prefix().str() + "_" + to_string(num) +
        "." + m2.suffix().str();
    }
    return chunkName;
}

void Client::sendChunks() {
    // Get file chunks
    fstream fs;
    fs.open(filePath, fstream::in | fstream::binary);
    int chunkSize;
    if ((fileSize % nodes.size()) == 0) {
        chunkSize = fileSize / nodes.size();
    } else {
        chunkSize = (fileSize / nodes.size()) + 1;
    }

    // Connect to Storage nodes one by one
    for (int i = 0; i < nodes.size(); i++) {
        setSocket(nodes[i]);
        createSocket();
        requestConnection();
        // Get chunk
        char *chunk = new char[chunkSize];
        fs.read(chunk, chunkSize);
        std::cout << "Port: " << nodes[i] << endl;
        std::cout << "ChunkSize: " << chunkSize << endl;
        std::cout << "Chunk Read: " << sizeof(chunk) << std::endl;
        std::cout << chunk << std::endl;
        // Parse file path to get chunk name
        string chunkName = parseFilePath(i);
        std::cout << "Chunk Name: " << chunkName << endl;
        // Send message type
        MessageType msgType = store;
        send(newSocket, (const void*)&msgType, sizeof(msgType), 0);
        // Send ChunkInfo message (i.e. name and size of chunk)
        ChunkInfo chunkInfo = {chunkName, chunkSize};
        int scs = send(newSocket, (void *)&chunkInfo, sizeof(chunkInfo), 0);
        // Send chunk
        int sc = send(newSocket, (void *)chunk, chunkSize, 0);
        std::cout << "Sent scs: " << scs << endl;
        std::cout << "Sent sc: " << sc << endl;
        
        delete[] chunk;
        closeConnection();
    }
    fs.close();
}