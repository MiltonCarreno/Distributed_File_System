#include "Storage.h"
#include "Message.cpp"
#include <vector>
#include <thread>
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;

// Number of connection requests waiting to be accepted
const int MAX_NUM_CONN_REQS = 2;
// Number of total connections to be accepted
const int MAX_NUM_CONN = 3;
// Number of heartbeats to send to controller
const int NUM_BEATS = 5;
// Number of seconds to wait between heartbeats
const int NUM_SECS = 5;

void chatFun(Storage *s, int connection, std::string path){    
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

        s->saveChunkFile(buff, chunkInfo.name, chunkInfo.size);
        delete [] buff;
    }
    // Close connection socket
    close(connection);
}

void beatHeart(Storage* s) {
    for (int i = 0; i<NUM_BEATS; i++) {
        cout << "\n##########################" << endl;
        // Create heartbeat connection with controller node 
        s->createHeartBeatSocket();
        // Request connection with controller node
        s->requestHeartBeatConnection();
        // Send heartbeat: chunk names and storage space
        s->sendBeat();
        // Close heartbeat socket
        s->closeHeartBeatSocket();
        cout << "##########################" << endl;
        // Wait 5 seconds before sending next heartbeat
        std::this_thread::sleep_for (std::chrono::seconds(NUM_SECS));
    }
}

int main(int agrc, char *argv[]) {
    // Create new storage node
    Storage storage(atoi(argv[1]), argv[2]);
    // Create new sockets
    storage.createSocket();
    // Bind store/query socket
    storage.bindSocket();
    // Delegate heartbeat
    vector<thread> threads;
    threads.emplace_back(thread(beatHeart, &storage));
    // Accept connection request and create new connection socket
    for (int i = 0; i<MAX_NUM_CONN; i++) {
        storage.listenConnection(MAX_NUM_CONN_REQS);
        int conn = storage.acceptConnection();
        // Delegate chatting/connections to individual threads
        threads.emplace_back(thread(chatFun, &storage, conn, argv[2]));
    }
    // Wait for all the threads to finish
    for (int i = 0; i<threads.size(); i++) {
        threads[i].join();
    }
    // Close listening/heartbeat sockets
    storage.closeConnection();
    return 0;
}