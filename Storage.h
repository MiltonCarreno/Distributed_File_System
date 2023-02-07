#ifndef STORAGE_H
#define STORAGE_H
#include <netinet/in.h>
#include <string>
#include <vector>
#include <mutex>

class Storage {
    private:
        std::mutex invMutex; // Mutex to lock access to inventory
        // Record of stored file names
        std::vector<std::string> inventory;
        // Storage path
        std::string path;
        // Port and space currently available
        int port, space;
        // heartbeat and store/query sockets
        int hbSocket, sqSocket, hbLen, sqLen, opt;
        // heartbeat and store/query address
        struct sockaddr_in hbAddress, sqAddress;

    public:
        Storage(int, std::string);
        void createSocket();
        void bindSocket();
        void listenConnection(int);
        int acceptConnection();
        void closeConnection();
        void createHeartBeatSocket();
        void requestHeartBeatConnection();
        void closeHeartBeatSocket();
        void sendBeat();
        void addChunkToInventory(std::string);
        void saveChunk(char*, std::string, int);
        void printInventory();
};
#endif