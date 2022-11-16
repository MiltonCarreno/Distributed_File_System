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
const int MAX_CONN = 2;

void chatFun(int connection){    
    MessageType msgType;
    recv(connection, (void *)&msgType, sizeof(msgType), 0);
    if (msgType != heartbeat) {
        int chunkSize = 0;
        recv(connection, (void *)&chunkSize, sizeof(chunkSize), 0);
        std::cout << "ChunkSize: " << chunkSize << endl;
        
        char *buff = new char[chunkSize];
        int r = recv(connection, (void *)buff, chunkSize, 0);
        std::cout << "Chunk Received: " << r << std::endl;
        std::cout << "Chunk len: " << strlen(buff) << std::endl;
        std::cout << "******************************" << std::endl;
        std::cout << "Chunk: " << buff << std::endl;
        std::cout << "******************************" << std::endl;
        delete [] buff;
    }

    // Close connection socket
    close(connection);
}

int main(int agrc, char *argv[]) {
    // Create new storage node
    Storage storage(atoi(argv[1]));
    // Create new sockets
    storage.createSocket();
    // Request connection with controller node
    storage.requestConnection();
    // Bind store/query socket
    storage.bindSocket();
    // Delegate heartbeat
    vector<thread> threads;
    threads.emplace_back(thread(&Storage::sendBeat, &storage));
    // Accept connection request and create new connection socket
    for (int i = 0; i<MAX_CONN; i++) {
        printf("In while loop!");
        storage.listenConnection(MAX_CONN_REQS);
        int conn = storage.acceptConnection();

        // Delegate chatting/connections to individual threads
        threads.emplace_back(thread(chatFun, conn));
    }

    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }

    // Close connection
    storage.closeConnection();
    return 0;
}