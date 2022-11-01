#include <string>
using namespace std;

enum MessageType {store, query, heartbeat};

struct Message {
    MessageType msgType;
    string fileName = "";
    int fileSize = 0;
};