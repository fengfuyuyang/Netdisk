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

    /* fd_set rdset; */
    char buf[16];

    int epfd = epoll_create(1);
    struct epoll_event event, evs[2];
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    
    setNonBlock(socketFd);
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = socketFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, socketFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyFdCount, i;

    while (1) {
        /* FD_ZERO(&rdset); */
        /* FD_SET(STDIN_FILENO, &rdset); */
        /* FD_SET(socketFd, &rdset); */

        /* ret = select(socketFd + 1, &rdset, NULL, NULL, NULL); */

        readyFdCount = epoll_wait(epfd, evs, 3, -1);
        for (i = 0; i < readyFdCount; ++i) {
            if (evs[i].events == EPOLLIN && evs[i].data.fd == STDIN_FILENO) {
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if (0 == ret) {
                    printf("Byebye\n");
                    goto end;
                }
                send(socketFd, buf, strlen(buf), 0);
            }
            if (evs[i].data.fd == socketFd) {
                while (1) {
                    bzero(buf, sizeof(buf));
                    ret = recv(socketFd, buf, sizeof(buf) - 1, 0);
                    if (0 == ret) {
                        printf("Byebye\n");
                        goto end;
                    } else if (-1 == ret) {
                        break;
                    } else {
                        printf("%s", buf);
                    }
                }
                /* printf("\n"); */
            }
        }
    }
end:
    close(socketFd);

    return 0;
}

