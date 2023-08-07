#include "../common/common.h"
#include "../common/tcp_server.h"

class User {
public:
    string name;
    bool online{};
    pthread_t tid{};
    int fd{};
};

string conf = "./server.conf";
User* client;
bool isRun = true;
int sum = 0;
int server_listen;

void signalHandler(int signum) {
    isRun = false;
    cout << "server close" << endl;
    close(server_listen);
    exit(signum);
}


bool check_online(const string& name) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && (client[i].name == name)) {
            cout << "W: " << name << " is online" << endl;
            return true;
        }
    }
    return false;
}

int find_sub() {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (!client[i].online) {
            return i;
        }
    }
    return -1;
}

void send_all(Msg msg) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online) {
            chat_send(msg, client[i].fd);
        }
    }
}

int check_name(const string& toName) {
    for (int i = 0; i < MAX_CLIENT; i++) {
        if (client[i].online && client[i].name == toName) {
            return i;
        }
    }
    return -1;
}

void *work(void *args) {
    int sub = *(int *)args;
    int clientFd = client[sub].fd;
    string clientName = client[sub].name;
    cout << "login: " << client[sub].name << endl;
    RecvMsg recvMsg;
    while (true) {
        recvMsg = chat_recv(clientFd);
        if (recvMsg.retVal < 0) {
            cout << "logout: " << client[sub].name << endl;
            close(clientFd);
            client[sub].online = false;
            sum--;
            return nullptr;
        }
        int recvFlag = recvMsg.msg.flag;
        string recvFrom = recvMsg.msg.from;
        string recvMessage = recvMsg.msg.message;
        cout << "from: " << recvFrom << ", message = " << recvMessage << endl;
        if (recvMessage == "#1") {
            cout << "[" << clientName << "] 查询在线人数" << endl;
            chat_send(Msg("server", 0, to_string(sum)), clientFd);
        } else if (recvMessage == "#2") {
            cout << "[" << clientName << "] 查询在线用户" << endl;
            string result;
            for (int i = 0; i < MAX_CLIENT; i++) {
                if (client[i].online) {
                    result.append(client[i].name + " ");
                }
            }
            chat_send(Msg("server", 0, result), clientFd);
        } else if (recvFlag == 1) {
            size_t pos = recvMessage.find(':');
            string toName = recvMessage.substr(1, pos - 1);
            string toMessage = recvMessage.substr(pos + 1);
            int ind = check_name(toName);
            if (ind != -1) {
                chat_send(Msg(recvFrom, 1, toMessage), client[ind].fd);
            } else {
                chat_send(Msg("server", 0, toName + "不在线"), clientFd);
            }
        } else {
            send_all(Msg(recvMsg.msg.from, 2, recvMsg.msg.message));
        }
    }
}

int main() {
    int port, fd;
    port = stoi(getValueFromConf(conf, "SERVER_PORT"));
    cout << "port = " << port << endl;
    client = new User[MAX_CLIENT];
    if ((server_listen = socket_create(port)) < 0) {
        return 1;
    }
    cout << "server init!" << endl;
    signal(SIGINT, signalHandler);
    while (isRun) {
        if ((fd = accept(server_listen, nullptr, nullptr)) < 0) {
            continue;
        }
        RecvMsg recvMsg = chat_recv(fd);
        if (recvMsg.retVal < 0) {
            close(fd);
            continue;
        }
        if (check_online(recvMsg.msg.from)) {
            chat_send(Msg("server", 3, "you have already login system!"), fd);
            close(fd);
            continue;
        }
        chat_send(Msg("server", 2, "welcome to this chat room!"), fd);
        sum++;
        int sub = find_sub();
        client[sub].online = true;
        client[sub].fd = fd;
        client[sub].name = recvMsg.msg.from;
        pthread_create(&client[sub].tid, nullptr, work, (void *)&sub);
    }
    return 0;
}