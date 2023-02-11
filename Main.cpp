#include "Controller.h"
#include "Message.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <thread>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_NUM_CONN_REQS = 10;
// Number of total connections to be accepted
const int MAX_NUM_CONN = 13;

void chatFun(Controller *serv, int connection) {
    MessageType msgType;
    read(connection, (void *)&msgType, sizeof(msgType));
    if (msgType != heartbeat) {
        FileInfo file;
        read(connection, (void *)&file, sizeof(file));
        cout << "\n******************************" << endl;
        cout << "----------Client Msg----------" << endl;
        cout << "Message type: " << enumStrings[msgType] << endl;
        cout << "Name of file: " << file.name << endl;
        cout << "Size of file: " << file.size << endl;
        cout << "******************************" << endl;
        // Get storage nodes with free space
        std::vector<int> nodes = serv->getFreeStorageNodes(file.size);
        cout << "\n-------------Nodes------------" << endl;
        for (const auto& n : nodes) {
            std::cout << n << "; ";
        }
        cout << "\n------------------------------\n" << endl;
        // Send storage node info one by one
        int numNodes = nodes.size();
        send(connection, (const void*)&numNodes, sizeof(numNodes), 0);
        for (int i = 0; i < numNodes; i++) {
            int node = nodes[i];
            send(connection, (const void*)&node, sizeof(node), 0);
        }

    } else {
        Heartbeat beat;
        read(connection, (void *)&beat, sizeof(beat));
        cout << "\n******************************" << endl;
        cout << "----------Storage Msg---------" << endl;
        cout << "Message type: " << enumStrings[msgType] << endl;
        cout << "Storage path: " << beat.data << endl;
        cout << "Storage size: " << beat.port << endl;
        cout << "******************************" << endl;
        serv->addStorageNode(beat.port, beat.space);
        // Get inventory from storage node
        int invSize;
        read(connection, (void *)&invSize, sizeof(invSize));
        std::vector<std::string> inv;
        cout << "Inv:" << endl;
        for (int i = 0; i < invSize; i++) {
            std::string chunkName;
            read(connection, (void *)&chunkName, sizeof(chunkName));
            inv.push_back(chunkName);
            cout << chunkName << endl;
        }
        cout << "End of Inv!" << endl;
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
    for (int i = 0; i<MAX_NUM_CONN; i++) {
        server.listenConnection(MAX_NUM_CONN_REQS);
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