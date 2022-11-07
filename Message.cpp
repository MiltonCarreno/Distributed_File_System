#include <string>
// Enum to specify the type of message
enum MessageType {store, query, heartbeat};
// Struct containing file info
struct FileInfo {
    std::string name = "";
    int size = 0;
};
// Struct containing chunk of data
struct Chunk {

};
// Struct containing storage nodes to send chunks to 
struct Nodes {
    std::string nodes;
};
// Struct containing storage node info
struct Heartbeat {
    std::string data = "";
    int port = 0;
};
// Struct containing message type and content (i.e. message)
struct Message {
    MessageType msgType;
    void *content;
};