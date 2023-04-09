#ifndef CLIENT_H
#define CLIENT_H
#include "Message.h"
#include <netinet/in.h>
#include <string>
#include <vector>
using namespace std;

class Client {
    private:
        string filePath;
        int fileSize, newSocket, addressLen;
        struct sockaddr_in address;
        std::vector<int> nodes;
        MessageType msgType;

    public:
        Client(int,string,string);
        void setSocket(int);
        void createSocket();
        void requestConnection();
        void closeConnection();
        void sendFileInfo();
        void getStorageNodes();
        string parseFilePath(int);
        void sendChunks();
};
#endif