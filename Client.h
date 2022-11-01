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
        Client(string);
        int createSocket();
        void requestConnection();
        void closeConnection();
        int getFileInfo(string);
        void printFileInfo();
};
#endif