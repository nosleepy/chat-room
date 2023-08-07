#include "head.h"

int socket_connect(const string& host, int port) {
    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host.c_str());
    if (connect(socketFd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cout << "socket connect fail!" << endl;
        return -1;
    }
    cout << "socket connect success!" << endl;
    return socketFd;
}