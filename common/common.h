#ifndef CHAT_ROOM_COMMON_H
#define CHAT_ROOM_COMMON_H

#include "head.h"

#define MAX_CLIENT 512

string getValueFromConf(const string& filename, const string& key);
int chat_send(Msg msg, int fd);
RecvMsg chat_recv(int fd);

#endif //CHAT_ROOM_COMMON_H