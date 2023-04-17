#ifndef STORAGE_H
#define STORAGE_H
#include <netinet/in.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>

/**
 * @file Storage.h
 * 
 * @brief Declaration of Storage Node class
 * 
 * @author Milton Carreno
 * 
 */
class Storage {
    private:
        // Mutex to lock access to inventory
        std::mutex invMutex;
        // Record of stored file names
        std::vector<std::string> inventory;
        std::map<std::string,std::vector<std::string>> inv;
        // Storage path
        std::string path;
        // Port and space currently available
        int port, space;
        // heartbeat and store/query sockets
        int hbSocket, sqSocket, hbLen, sqLen, opt;
        // heartbeat and store/query address
        struct sockaddr_in hbAddress, sqAddress;

    public:
        Storage(int,std::string);
        void createSocket();
        void bindSocket();
        void listenConnection(int);
        int acceptConnection();
        void closeConnection();
        void createHeartBeatSocket();
        void requestHeartBeatConnection();
        void closeHeartBeatSocket();
        void sendBeat();
        void addChunkToInventory(std::string,std::string);
        void saveChunkFile(char*,std::string,std::string,int);
        void printInventory();
};
#endif