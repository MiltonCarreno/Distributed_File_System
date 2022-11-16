#include <string>
// Enum to specify the type of message
enum MessageType {store, query, heartbeat};
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