#include "Storage.h"

int main(int agrc, char *argv[]) {
    // Create new storage node
    Storage storage;
    // Create new socket
    storage.createSocket();
    // Request connection with controller node
    storage.requestConnection();
    // Send storage node info
    storage.sendMsg();
    // Close connection
    storage.closeConnection();
}