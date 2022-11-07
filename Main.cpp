#include "Controller.h"
#include "Message.cpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <thread>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_CONN_REQS = 1;
// Number of total connections to be accepted
const int MAX_CONN = 3;

void chatFun(int connection){    
    // Message msg;
    MessageType msgType;
    read(connection, (void *)&msgType, sizeof(msgType));
    if (msgType != heartbeat) {
        FileInfo file;
        read(connection, (void *)&file, sizeof(file));
        cout << "******************************" << endl;
        cout << "|---------Client Msg----------" << endl;
        cout << "Message type: " << msgType << endl;
        cout << "Size of name: " << file.name << endl;
        cout << "Size of file: " << file.size << endl;
        cout << "******************************" << endl;
    } else {
        Heartbeat beat;
        read(connection, (void *)&beat, sizeof(beat));
        cout << "******************************" << endl;
        cout << "|---------Storage Msg---------" << endl;
        cout << "Message type: " << msgType << endl;
        cout << "Size of name: " << beat.data << endl;
        cout << "Size of file: " << beat.port << endl;
        cout << "******************************" << endl;
    }

    // Close connection socket
    close(connection);
}

int main(int argc, char *argv[]) {
    // Create Controller
    Controller server;

    // Create and bind new listening socket
    server.createSocket();
    server.bindSocket();

    // Accept connection request and create new connection socket
    vector<thread> threads;
    for (int i = 0; i<MAX_CONN; i++) {
        printf("In while loop!");
        server.listenConnection(MAX_CONN_REQS);
        int conn = server.acceptConnection();

        // Delegate chatting/connections to individual threads
        threads.emplace_back(thread(chatFun, conn));
    }

    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }
  
    // Close listening socket
    server.shutdownSocket();
    return 0;
}