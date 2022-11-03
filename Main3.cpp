#include "Storage.h"
#include "Message.cpp"
#include <vector>
#include <thread>
#include <iostream>
#include <unistd.h>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_CONN_REQS = 1;
// Number of total connections to be accepted
const int MAX_CONN = 3;

void chatFun(int connection){    
    Message msg;
    read(connection, (void *)&msg, sizeof(msg));
    if (msg.msgType != heartbeat) {
        cout << "******************************" << endl;
        cout << "|---------Client Msg----------" << endl;
        cout << "Message type: " << msg.msgType << endl;
        cout << "Size of name: " << msg.fileName << endl;
        cout << "Size of file: " << msg.fileSize << endl;
        cout << "******************************" << endl;
    }

    // Close connection socket
    close(connection);
}

int main(int agrc, char *argv[]) {
    // Create new storage node
    Storage storage;
    // Create new socket
    storage.createSocket();
    // Request connection with controller node
    storage.requestConnection();
    // Delegate heartbeat
    vector<thread> threads;
    threads.emplace_back(thread(&Storage::sendBeat, &storage));
    // Accept connection request and create new connection socket
    for (int i = 0; i<MAX_CONN; i++) {
        printf("In while loop!");
        server.listenConnection(MAX_CONN_REQS);
        int conn = server.acceptConnection();

        // Delegate chatting/connections to individual threads
        threads.emplace_back(thread(chatFun, conn));
    }

    // Close connection
    storage.closeConnection();
}