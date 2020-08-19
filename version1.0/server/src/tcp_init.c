/*******************************************************************************
    > File Name: tcp_init.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "../include/head.h"

int tcpInit(int *sfd, char* ip, int port) {
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");

    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(port);
    serAddr.sin_addr.s_addr = inet_addr(ip);

    int reuse = 1;
    int ret;

    /* 设置端口可重连 */
    ret = setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(socketFd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");
    listen(socketFd, 10);
    *sfd = socketFd;

    return 0;
}

