#include <string>

enum MessageType {store, query, heartbeat};

struct Message {
    MessageType msgType;
    std::string fileName = "";
    int fileSize = 0;
};