/*******************************************************************************
    > File Name: client.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/
#define _GNU_SOURCE
#include "../include/head.h"

int main()
{
    Config_t config = {};
    if (!configInfo(&config)) {
        puts("Configuration error, please check the client.conf!");
        return -1;
    }

#if 0
    printf("ip: %s\nport: %d\n", config.ip, config.port);

#endif
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

    while (-1 == login(socketFd)) {
        printf("Login failed, please try again\n\n");
    }

    /* int fd; */
    /* int dataLen; */
    /* char buf[1000] = {0}; */
    /* recvCycle(socketFd, &dataLen, 4); */
    /* recvCycle(socketFd, buf, dataLen);//接收文件名 */
    /* fd = open(buf, O_CREAT|O_RDWR, 0666); */
    /* ERROR_CHECK(fd, -1, "open"); */
    /* //接收文件大小 */
    /* off_t fileSize, downLoadSize = 0; */
    /* recvCycle(socketFd, &dataLen, 4); */
    /* recvCycle(socketFd, &fileSize, dataLen); */
    /* printf("fileSize = %ld\n", fileSize); */
    /* struct timeval start,end; */
    /* int fds[2]; */
    /* pipe(fds); */
    /* gettimeofday(&start, NULL); */
    /* while (downLoadSize < fileSize) { */
    /*     //第一次splice把数据从socketFd读到管道， */
    /*     ret = splice(socketFd, NULL, fds[1], NULL, 65536, SPLICE_F_MOVE|SPLICE_F_MORE); */
    /*     //当对端断开时，splice返回值为0 */
        /* if(ret == 0) */
        /*     break; */
        /* ERROR_CHECK(ret, -1, "splice"); */
        /* //第二次splice把数据从管道读到文件描述符 */
        /* splice(fds[0], NULL, fd, NULL, ret, SPLICE_F_MOVE|SPLICE_F_MORE); */
        /* downLoadSize += ret; */
    /* } */
    /* gettimeofday(&end, NULL); */
    /* printf("use time = %ld\n", (end.tv_sec - start.tv_sec)*1000000 + end.tv_usec - start.tv_usec); */
    /* close(fd); */
    /* close(socketFd); */




    /* fd_set rdset; */

    fflush(stdout);

    int epfd = epoll_create(1);
    epollInAdd(epfd, STDIN_FILENO);
    epollInAdd(epfd, socketFd);

    struct epoll_event evs[2];

    int readyFdCount, i;
    train_t train;

    int cmdFlag;
    char cmd[8] = {0};
    char nowpath[512] = "/";
    printf("~%s$ ", nowpath);
    /* fflush(stdout); */
    while (1) {
        printf("while\n");
        readyFdCount = epoll_wait(epfd, evs, 2, -1);
        for (i = 0; i < readyFdCount; ++i) {
            if (evs[i].data.fd == STDIN_FILENO) {
                /* printf("~%s$ ", nowpath); */
                bzero(&train, sizeof(train));
                ret = read(STDIN_FILENO, train.buf, sizeof(train.buf));
                train.buf[strlen(train.buf) - 1] = '\0';
                if (0 == (train.dataLen = strlen(train.buf))) {
                    break;
                }
                sscanf(train.buf, "%s", cmd);
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
                    puts(nowpath);
                } else if (0 == strcmp(cmd, "exit")) {
                    goto end;
                } else {
                    break;
                }
                send(socketFd, &train, train.dataLen + 4, 0);
                printf("\nsend: %s\n", train.buf);
            }
            if (evs[i].data.fd == socketFd) {
                printf("request\n");
                ret = request(socketFd, cmdFlag, nowpath);
                if (-1 == ret) {
                    goto end;
                }
            }
        }
    }
end:
    close(socketFd);

    return 0;
}

