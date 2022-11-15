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

void chatFun(Controller *serv, int connection){    
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

        // std::map<int,int> nodes = serv->getFreeStorageNodes(file.size);
        // cout << "\n-------------Nodes--------------" << endl;
        // for (const auto& [key, val] : nodes) {
        //     std::cout << '[' << key << "] = " << val << "; ";
        // }
        // cout << "\n------------------------------\n" << endl;
        // int mapSize = nodes.size();
        // send(connection, (const void*)&mapSize, sizeof(mapSize), 0);
        // int s = send(connection, (const void*)&nodes, sizeof(nodes), 0);
        // cout << "Map Bytes Size: " << sizeof(nodes) << endl;
        // cout << "Bytes Sent: " << s << endl;

        std::vector<int> nodes = serv->getFreeStorageNodesV(file.size);
        cout << "\n-------------Nodes--------------" << endl;
        for (const auto& n : nodes) {
            std::cout << n << "; ";
        }
        cout << "\n------------------------------\n" << endl;
        int mapSize = nodes.size();
        send(connection, (const void*)&mapSize, sizeof(mapSize), 0);

        for (int i = 0; i < mapSize; i++) {
            int n = nodes[i];
            send(connection, (const void*)&n, sizeof(n), 0);        
        }

    } else {
        Heartbeat beat;
        read(connection, (void *)&beat, sizeof(beat));
        cout << "******************************" << endl;
        cout << "|---------Storage Msg---------" << endl;
        cout << "Message type: " << msgType << endl;
        cout << "Size of name: " << beat.data << endl;
        cout << "Size of file: " << beat.port << endl;
        cout << "******************************" << endl;
        serv->addStorageNode(beat.port, beat.space);
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
        threads.emplace_back(thread(chatFun, &server, conn));
    }

    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }
  
    // Close listening socket
    server.shutdownSocket();
    return 0;
}