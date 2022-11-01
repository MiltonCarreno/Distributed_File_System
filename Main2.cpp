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

int main(int argc, char const* argv[]) {
    // Create new client
    Client client(argv[1]);

    // Creat new client socket
    client.createSocket();

    // Request connection with controller
    client.requestConnection();

    // Send file info
    client.sendMsg();
    
    // Close connection
    client.closeConnection();
    return 0;
}