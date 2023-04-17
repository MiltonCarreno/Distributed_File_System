#ifndef CLIENT_H
#define CLIENT_H
#include "Message.h"
#include <netinet/in.h>
#include <string>
#include <vector>
#include <tuple>

/**
 * @file Client.h
 * 
 * @brief Declaration of Client Node class
 * 
 * @author Milton Carreno
 * 
 */
class Client {
    private:
        std::string filePath, fileName;
        int fileSize, newSocket, addressLen;
        struct sockaddr_in address;
        std::vector<int> nodes;
        MessageType msgType;

    public:
        Client(int,MessageType,std::string);
        void setSocket(int);
        void createSocket();
        void requestConnection();
        void closeConnection();
        void sendFileInfo();
        void getStorageNodes();
        std::string getFileName(std::string);
        std::string getChunkName(int);
        void sendChunks();
};
#endif