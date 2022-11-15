#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
#include <string>
using namespace std;

class Client {
    private:
        string filePath;
        int fileSize, newSocket, addressLen;
        struct sockaddr_in address;

    public:
        Client(int, string);
        void createSocket();
        void requestConnection();
        void closeConnection();
        void printFileInfo();
        void sendStoreMsg();
        void getStorageNodes();
};
#endif