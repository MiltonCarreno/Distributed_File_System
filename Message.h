#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
// Enum to specify the type of message
enum MessageType {store, query, heartbeat};
// Array of enum value to corresponding enum string
// "const" provides internal linkage so that Main3.o & Storage.o can be linked
const std::string MessageTypeStrings[] {"Store", "Query", "Heartbeat"};
// Struct containing file info (used in store and query reqs)
struct FileInfo {
    std::string fileName = "";
    std::string chunkName = "";
    int size = 0;
};
// Struct containing storage node info
struct Heartbeat {
    std::string path = "";
    int port = 0;
    int space = 0;
};
#endif