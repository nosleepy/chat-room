#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in serv_addr;
    socklen_t clnt_adr_sz;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>
", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket() error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    while (1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "q
") || !strcmp(message, "Q
"))
            break;

        sendto(sock, message, strlen(message), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    }

    close(sock);
    return 0;
}

