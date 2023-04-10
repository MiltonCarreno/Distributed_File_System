#include "Client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

/**
 * @brief Determines if Client wants to send a store or query message
 * 
 * @param arg 
 * @return type of message (i.e. MessageType)
 */
MessageType getRequestType(std::string arg) {
    MessageType msgType;
    if (arg == "-s") {  // Store request
        msgType = MessageType::store;
    } else if (arg == "-q") {   // Query request
        msgType = MessageType::query;
    }
    return msgType;
}

/**
 * @brief Main function for Client
 * 
 * @param argc Two arguments are expected
 * @param argv Controller Port & File Path
 * @return int 
 */
int main(int argc, char *argv[]) {
    // Determine if it's a store or query request
    MessageType msg = getRequestType(argv[2]);

    // Create new client node
    Client client(atoi(argv[1]), msg, argv[3]);

    // Creat new socket
    client.createSocket();

    // Request connection with Controller node
    client.requestConnection();

    // Send file info
    client.sendFileInfo();

    // Get Storage nodes from Controller
    client.getStorageNodes();
    
    // Send chunks to Storage nodes
    client.sendChunks();

    // Close connection
    client.closeConnection();
    return 0;
}