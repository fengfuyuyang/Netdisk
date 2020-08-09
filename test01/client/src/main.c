/*******************************************************************************
    > File Name: client.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "head.h"

int main()
{
    Config_t config = {};
    if (!configInfo(&config)) {
        puts("Configuration error, please check the client.conf!");
        return -1;
    }

#if 0
    printf("ip: %s\nport: %s\n", config.ip, config.port);

#endif
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");

    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(config.port));
    serAddr.sin_addr.s_addr = inet_addr(config.ip);

    int ret;
    ret = connect(socketFd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");

    fd_set rdset;
    char buf[1024];

    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socketFd, &rdset);

        ret = select(socketFd + 1, &rdset, NULL, NULL, NULL);

        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));

                if (0 == ret) {
                    printf("beybey\n");
                    break;
                }

                send(socketFd, buf, strlen(buf) - 1, 0);
            }

            if (FD_ISSET(socketFd, &rdset)) {
                bzero(buf, sizeof(buf));
                ret = recv(socketFd, buf, sizeof(buf) - 1, 0);

                if (0 == ret) {
                    printf("byebye\n");
                    break;
                }

                printf("%s\n", buf);
            }
        }
    }

    close(socketFd);

    return 0;
}

