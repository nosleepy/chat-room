#ifndef CHAT_ROOM_HEAD_H
#define CHAT_ROOM_HEAD_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstring>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <thread>

using namespace std;

class Msg {
public:
    char from[20]{};
    int flag{};
    char message[512]{};
    Msg() = default;
    Msg(const string& from, int flag, const string& message) {
        strcpy(this->from, from.c_str());
        this->flag = flag;
        strcpy(this->message, message.c_str());
    }
};

class RecvMsg {
public:
    Msg msg{};
    int retVal{};
};

#endif //CHAT_ROOM_HEAD_H