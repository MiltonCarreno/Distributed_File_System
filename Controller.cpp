#include "Controller.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
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
    if (bind(newSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("\nBound Socket\n");
}

bool Controller::listenConnection(int queueLen) {
    bool heard = (listen(newSocket, queueLen) == 0);
    if (!heard) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("\nHeard Connection\n");
    return heard;
}

int Controller::acceptConnection() {
    int connection, conn = 0;
    if ((connection = accept(newSocket, (struct sockaddr*)&address, (socklen_t*)&addressLen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    conn = connection;
    printf("\nAccepted Connection\n");
    return conn;
}

void Controller::shutdownSocket() {
    shutdown(newSocket, SHUT_RDWR);
    printf("\nShutdown Listening Socket!\n");
}

