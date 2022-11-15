// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Client.h"
using namespace std;

int main(int argc, char *argv[]) {
    // Create new client node
    Client client(atoi(argv[1]), argv[2]);

    // Creat new socket
    client.createSocket();

    // Request connection with controller node
    client.requestConnection();

    // Send file info
    client.sendStoreMsg();
    
    // Send chunks to Controller-provided storage nodes

    // Close connection
    client.closeConnection();
    return 0;
}