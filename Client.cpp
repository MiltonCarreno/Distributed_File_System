#include "Client.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <regex>
#define PORT 9090
const char* LOCAL_HOST = "127.0.0.1";

/**
 * @brief Constructs a new Client object. 
 * Connects to Controller at given port and stores file.
 * 
 * @param port Port to which to connect with Controller
 * @param msg Message type to be sent to Controller
 * @param file File to be sent to Controller for storage
 */
Client::Client(int port, MessageType msg, std::string path) 
: filePath(path), fileName(getFileName(path)), msgType(msg) {
    if (msg == MessageType::store) {        // Store request
        std::fstream fs(path, std::fstream::in | std::fstream::binary);
        if (fs.is_open()) {
            fs.seekg(0, fs.end);
            fileSize = fs.tellg();          // Get file size
            fs.seekg(0, fs.beg);
            fs.close();
        } else {
            std::cout << "\nFile didn't open" << std::endl;
        }
    } else if (msg == MessageType::query) { // Query request
        fileSize = 0;
    }
    setSocket(port);
    std::cout << "\nThis is the constructor" << std::endl;
}

/**
 * @brief Sets socket information
 * 
 * @param port Port to connect to
 */
void Client::setSocket(int port) {
    // Set socket info
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    addressLen = sizeof(address);

    if (inet_pton(AF_INET, LOCAL_HOST, &address.sin_addr) <= 0) {
        std::cout << "\nInvalid address/ Address not supported" << std::endl;
    }
}

/**
 * @brief Creates new socket for Client
 * 
 */
void Client::createSocket() {
    // Creating socket file descriptor
    if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "\nCreated Socket" << std::endl;
}

/**
 * @brief Uses Client socket to request connection
 * 
 */
void Client::requestConnection() {
    int connection = connect(newSocket, 
    (struct sockaddr*)&address, (socklen_t)addressLen);
    if (connection < 0) {
        perror("Connection request error");
        exit(EXIT_FAILURE);
    }
    std::cout << "\nConnection Request Accepted" << std::endl;
}

/**
 * @brief Closes the Client socket connection
 * 
 */
void Client::closeConnection() {
    int connection = close(newSocket);
    if (connection < 0) {
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Sends file info (path, size) to the controller
 * 
 */
void Client::sendFileInfo() {
    // Send file info to Controller
    FileInfo msg = {fileName, "", fileSize};
    send(newSocket, (const void*)&msgType, sizeof(msgType), 0);
    send(newSocket, (const void*)&msg, sizeof(msg), 0);
}

/**
 * @brief Obtain available nodes from Controller.
 * 
 */
void Client::getStorageNodes() {
    // Get number of available storage nodes from Controller
    int numNodes = 0;
    read(newSocket, (void *)&numNodes, sizeof(numNodes));
    // Get one by one the storage nodes
    std::cout << "\n-------------Nodes------------" << std::endl;
    for (int i = 0; i < numNodes; i++) {
        int node;
        read(newSocket, (void *)&node, sizeof(node));
        nodes.push_back(node);
        std::cout << node << "; ";
    }
    std::cout << "\n------------------------------\n" << std::endl;
}

/**
 * @brief Extracts the file name from the file path.
 * 
 * @param path File path
 * @return std::string File name
 */
std::string Client::getFileName(std::string path) {
    std::smatch m;
    std::regex e("[^/]+\\w+$");
    std::regex_search(path, m, e);
    std::string fileName = m.str();
    return fileName;
}

/**
 * @brief Generates chunk name from file name
 * 
 * @param chunkNum Idex indicating chunk order
 * @return std::string Chunk name (e.g. file_7.txt)
 */
std::string Client::getChunkName(int chunkNum) {
    std::smatch m;
    std::regex e("\\.");
    std::regex_search(fileName, m, e);    
    return m.prefix().str() + "_" + 
    std::to_string(chunkNum) + "." + m.suffix().str();
}

/**
 * @brief Creates and sends chunks to their respective Storage nodes.
 * 
 */
void Client::sendChunks() {
    // Get file chunks
    std::fstream fs(filePath, std::fstream::in | std::fstream::binary);
    int chunkSize;
    if ((fileSize % nodes.size()) == 0) {
        chunkSize = fileSize / nodes.size();
    } else {
        chunkSize = (fileSize / nodes.size()) + 1;
    }
    // Connect to Storage nodes one by one
    for (int i = 0; i < nodes.size(); i++) {
        setSocket(nodes[i]);
        createSocket();
        requestConnection();
        // Get chunk from file
        char *chunk = new char[chunkSize];
        fs.read(chunk, chunkSize);
        std::cout << "Port: " << nodes[i] << std::endl;
        std::cout << "ChunkSize: " << chunkSize << std::endl;
        std::cout << "Chunk Read: " << sizeof(chunk) << std::endl;
        std::cout << chunk << std::endl;
        // Parse file path to get chunk name
        std::string chunkName = getChunkName(i);
        std::cout << "File Name: " << fileName << std::endl;
        std::cout << "Chunk Name: " << chunkName << std::endl;
        // Send message type
        MessageType msgType = store;
        send(newSocket, (const void*)&msgType, sizeof(msgType), 0);
        // Send FileInfo message (i.e. name and size of chunk)
        FileInfo chunkInfo = {fileName, chunkName, chunkSize};
        int scs = send(newSocket, (void *)&chunkInfo, sizeof(chunkInfo), 0);
        // Send chunk
        int sc = send(newSocket, (void *)chunk, chunkSize, 0);
        std::cout << "Sent scs: " << scs << std::endl;
        std::cout << "Sent sc: " << sc << std::endl;
        // Free memory
        delete[] chunk;
        closeConnection();
    }
    fs.close();
}