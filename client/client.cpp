#include "../common/common.h"
#include "../common/tcp_client.h"
#include "../common/color.h"

string conf = "./client.conf";
string logFile;
int socketFd;
int flag;
string myName;
string input;
RecvMsg recvMsg;

void signalHandler(int signum) {
    cout << "client close" << endl;
    close(socketFd);
    exit(signum);
}

void read_task() {
    ofstream outfile("./" + myName + "_" + logFile, ios::app);
    while (true) {
        recvMsg = chat_recv(socketFd);
        if (recvMsg.retVal < 0) {
            cout << "error in server!" << endl;
            outfile.close();
            raise(SIGINT);
            break;
        }
        if (recvMsg.msg.flag == 0) {
            cout << YELLOW << "[server]" << NONE << " " << recvMsg.msg.message << endl;
            outfile << "recv: [notify] <server> {" << recvMsg.msg.message << "}" << endl;
        } else if (recvMsg.msg.flag == 1) {
            cout << GREEN << "<" << recvMsg.msg.from << ">" << NONE << " " << recvMsg.msg.message << endl;
            outfile << "recv: [one] " << "<" << recvMsg.msg.from << "> {" << recvMsg.msg.message << "}" << endl;;
        } else if (recvMsg.msg.flag == 2) {
            cout << BLUE << "[" << recvMsg.msg.from << "]" << NONE << " " << recvMsg.msg.message << endl;
            outfile << "recv: [all] " << "<" << recvMsg.msg.from << "> {" << recvMsg.msg.message << "}" << endl;
        } else if (recvMsg.msg.flag == -1) {
            cout << RED << "错误消息" << NONE << endl;
            outfile << "recv: [error]" << endl;
        }
    }
}

void write_task() {
    ofstream outfile("./" + myName + "_" + logFile, ios::app);
    while (true) {
        cout << "please input message:" << endl;
        cin >> input;
        getchar();
        if (input == "exit") {
            cout << "client exit!" << endl;
            outfile.close();
            raise(SIGINT);
            break;
        }
        if (input[0] == '#') {
            flag = 0;
        } else if (input[0] == '@') {
            flag = 1;
        } else {
            flag = 2;
        }
        outfile << "send: " << input << endl;
        chat_send(Msg(myName, flag, input), socketFd);
    }
}

int main() {
    int port = stoi(getValueFromConf(conf, "SERVER_PORT"));
    string ip = getValueFromConf(conf, "SERVER_IP");
    logFile = getValueFromConf(conf, "LOG_FILE");
    myName = getValueFromConf(conf, "MY_NAME");
    cout << "client name = " << myName << endl;

    if ((socketFd = socket_connect(ip, port)) < 0) {
        return 1;
    }

    if (chat_send(Msg(myName, 2, "connect"), socketFd) < 0) {
        return 2;
    }

    recvMsg = chat_recv(socketFd);
    if (recvMsg.retVal < 0) {
        cout << "Error!" << endl;
        return 1;
    }
    cout << "Server: " << recvMsg.msg.message << endl;

    if (recvMsg.msg.flag == 3) {
        close(socketFd);
        return 1;
    }

    signal(SIGINT, signalHandler);
    thread read_thread(read_task);
    thread write_thread(write_task);
    read_thread.join();
    write_thread.join();

    return 0;
}