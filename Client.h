#ifndef CLIENT_H
#define CLIENT_H
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

    public:
        Client(int, string);
        void setSocket(int);
        void createSocket();
        void requestConnection();
        void closeConnection();
        void sendFileInfo();
        void getStorageNodes();
        void sendChunks();
};
#endif