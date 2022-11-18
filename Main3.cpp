#include "Storage.h"
#include "Message.cpp"
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_CONN_REQS = 1;
// Number of total connections to be accepted
const int MAX_CONN = 2;

void chatFun(int connection, std::string path){    
    MessageType msgType;
    recv(connection, (void *)&msgType, sizeof(msgType), 0);
    if (msgType != heartbeat) {
        // Get ChunkInfo (i.e. name and size) 
        ChunkInfo chunkInfo;
        int scs = recv(connection, (void *)&chunkInfo, sizeof(chunkInfo), 0);
        // Get chunk
        char *buff = new char[chunkInfo.size];
        int r = recv(connection, (void *)buff, chunkInfo.size, 0);
        std::cout << "Bytes Received: " << r << std::endl;
        std::cout << "Chunk File Name: " << chunkInfo.name << std::endl;
        std::cout << "Chunk File Size: " << chunkInfo.size << std::endl;
        std::cout << "Chunk len: " << strlen(buff) << std::endl;
        std::cout << "******************************" << std::endl;
        std::cout << "Chunk: " << buff << std::endl;
        std::cout << "******************************" << std::endl;

        string chunkPath = path + "/" + chunkInfo.name;

        fstream fs(chunkPath, fstream::out | fstream::binary);
        fs.write(buff, chunkInfo.size);
        fs.close();
        delete [] buff;
    }

    // Close connection socket
    close(connection);
}

int main(int agrc, char *argv[]) {
    // Create new storage node
    Storage storage(atoi(argv[1]), argv[2]);
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
        threads.emplace_back(thread(chatFun, conn, argv[2]));
    }

    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }

    // Close connection
    storage.closeConnection();
    return 0;
}