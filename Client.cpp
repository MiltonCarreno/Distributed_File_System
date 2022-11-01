#include "Client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
const char* LOCAL_HOST = "127.0.0.1";

Client::Client(string file_path) {
    // Set file info
    filePath = file_path;
    fileSize = getFileInfo(file_path);

    // Set socket info
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    addressLen = sizeof(address);

    if (inet_pton(AF_INET, LOCAL_HOST, &address.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
    }

    printf("\nThis is the constructor\n");
}

int Client::createSocket() {
    // Creating socket file descriptor
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("\nCreated Socket\n");
    return newSocket;
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

int Client::getFileInfo(string file) {
    fstream fs;
    int length = 0;
    fs.open(file, fstream::in | fstream::binary);
    if (fs.is_open()) {
        cout << "File opend!" << endl;
        fs.seekg(0, fs.end);
        length = fs.tellg();
        fs.seekg(0, fs.beg);
        // cout << "The length is "<< length << endl;
        fs.close();
    } else {
        cout << "File didn't open" << endl;
    }
    return length;
}

void Client::printFileInfo() {
    cout << "File: " << filePath << endl;
    cout << "Size: " << fileSize << endl; 
}