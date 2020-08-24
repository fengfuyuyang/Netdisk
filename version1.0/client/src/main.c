/*******************************************************************************
    > File Name: client.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/
#include "../include/head.h"

int main()
{
    Config_t config = {};
    if (!configInfo(&config)) {
        puts("Configuration error, please check the client.conf!");
        return -1;
    }

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");

    struct sockaddr_in serAddr;
    bzero(&serAddr, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(config.port);
    serAddr.sin_addr.s_addr = inet_addr(config.ip);

    int ret;
    ret = connect(socketFd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");

    char usrname[20] = {0};
    while (-1 == login(socketFd, usrname)) {
        printf("Login failed, please try again\n\n");
    }

    int epfd = epoll_create(1);
    epollInAdd(epfd, STDIN_FILENO);
    epollInAdd(epfd, socketFd);

    struct epoll_event evs[2];

    int readyFdCount, i;
    train_t train;

    int cmdFlag;
    char cmd[8] = {0};
    char pathname[128] = {0};
    char nowpath[512] = "/";
    printf("%s:~%s$ ", usrname, nowpath);
    fflush(stdout);
    while (1) {
        readyFdCount = epoll_wait(epfd, evs, 2, -1);
        for (i = 0; i < readyFdCount; ++i) {
            if (evs[i].data.fd == STDIN_FILENO) {
                bzero(&train, sizeof(train));
                ret = read(STDIN_FILENO, train.buf, sizeof(train.buf));
                train.buf[strlen(train.buf) - 1] = '\0';
                if (0 == (train.dataLen = strlen(train.buf))) {
                    printf("~%s$ ", nowpath);
                    fflush(stdout);
                    break;
                }
                sscanf(train.buf, "%s%s", cmd, pathname);
                if (0 == strcmp(cmd, "cd")) {
                    cmdFlag = CD;
                } else if (0 == strcmp(cmd, "ls")) {
                    cmdFlag = LS;
                } else if (0 == strcmp(cmd, "puts")) {
                    cmdFlag = PUTS;
                } else if (0 == strcmp(cmd, "gets")) {
                    cmdFlag = GETS;
                } else if (0 == strcmp(cmd, "mkdir")) {
                    cmdFlag = MKDIR;
                } else if (0 == strcmp(cmd, "rm") || 0 == strcmp(cmd, "remove")) {
                    cmdFlag = RM;
                } else if (0 == strcmp(cmd, "pwd")) {
                    cmdFlag = PWD;
                } else if (0 == strcmp(cmd, "exit")) {
                    goto end;
                } else {
                    printf("%s: commmond not found\n", cmd);
                    printf("%s:~%s$ ", usrname, nowpath);
                    fflush(stdout);
                }
                send(socketFd, &train, train.dataLen + 4, 0);
            }
            if (evs[i].data.fd == socketFd) {
                ret = request(socketFd, pathname, cmdFlag, nowpath);
                if (-1 == ret) {
                    printf("\nServer disconnected\n");
                    goto end;
                }
                printf("%s:~%s$ ", usrname, nowpath);
                fflush(stdout);
            }
            if (cmdFlag == PUTS) {
                putsCmd(socketFd, pathname);
                printf("%s:~%s$ ", usrname, nowpath);
                fflush(stdout);
            }
        }
    }
end:
    close(socketFd);

    return 0;
}

