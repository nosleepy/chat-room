#include "head.h"

string getValueFromConf(const string& filename, const string& key) {
    ifstream file(filename);
    map<string, string> config;
    string line;
    string result;

    if (file.is_open()) {
        while (getline(file, line)) {
            istringstream iss(line);
            string currentKey;
            getline(iss, currentKey, '=');
            if (currentKey == key) {
                getline(iss, result);
                break;
            } else {
                config[currentKey] = "";
            }
        }
        file.close();
    } else {
        cerr << "Failed to open conf file." << endl;
        return "";
    }

    return result;
}

int chat_send(Msg msg, int fd) {
    if (send(fd, (void *)&msg, sizeof(Msg), 0) <= 0) {
        return -1;
    }
    return 0;
}

RecvMsg chat_recv(int fd) {
    RecvMsg tmp;
    if (recv(fd, &tmp.msg, sizeof(Msg), 0) <= 0) {
        tmp.retVal = -1;
    }
    return tmp;
}