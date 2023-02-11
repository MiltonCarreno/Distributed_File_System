#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
// Enum to specify the type of message
enum MessageType {store, query, heartbeat};
// Array of enum value to corresponding enum string
// "const" provides internal linkage so that Main3.o & Storage.o can be linked
const std::string enumStrings[] {"Store", "Query", "Heartbeat"};
// Struct containing file info
struct FileInfo {
    std::string name = "";
    int size = 0;
};
// Struct containing chunk of data
struct ChunkInfo {
    std::string name = "";
    int size = 0;
};
// Struct containing storage node info to be used to send chunks to 
struct StorageNodeInfo {
    int port;
    int space;
};
// Struct containing storage node info
struct Heartbeat {
    std::string data = "";
    int port = 0;
    int space = 0;
};
// Struct containing message type and content (i.e. message)
struct Message {
    MessageType msgType;
    void *content;
};
#endif