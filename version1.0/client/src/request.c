/*******************************************************************************
    > File Name: request.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-23
 ******************************************************************************/

#include "../include/head.h"

int request(int socketFd, char* nowpath, int CMD, char* pathname) {

    int dataLen, ret;
    char buf[512] = {0};
    switch (CMD) {
    case CD:
        ret = endRcv(socketFd);
        if (-1 == ret) {
            return -1;
        }
        /* 每次进入新的目录，更新客户端当前目录显示 */
        recvCycle(socketFd, &dataLen, 4);
        recvCycle(socketFd, buf, dataLen);
        strcpy(nowpath, buf);
        break;
    case LS:
        ret = recvCycle(socketFd, &dataLen, 4);
        if (-1 == ret) {
            return -1;
        }
        while (dataLen) {
            recvCycle(socketFd, buf, dataLen);
            puts(buf);
            bzero(buf, sizeof(buf));
            recvCycle(socketFd, &dataLen, 4);
        }
        break;
    case GETS:
        getsCmd(socketFd);
        break;
    case MKDIR:
        ret = endRcv(socketFd);
        if (-1 == ret) {
            return -1;
        }
        break;
    case RM:
        ret = recvCycle(socketFd, &dataLen, 4);
        if (-1 == ret) {
            return -1;
        }
        while (dataLen) {
            recvCycle(socketFd, buf, dataLen);
            puts(buf);
            bzero(buf, sizeof(buf));
            recvCycle(socketFd, &dataLen, 4);
        }
        break;
    case PWD:
        ret = recvCycle(socketFd, &dataLen, 4);
        if (-1 == ret) {
            return -1;
        }
        recvCycle(socketFd, buf, dataLen);
        puts(buf);
        break;
    default:
        /* ret = recvCycle(socketFd, &dataLen, 4); */
        /* if (-1 == ret) { */
        /*     return -1; */
        /* } */
        /* recvCycle(socketFd, buf, dataLen); */
        /* puts(buf); */
        break;
    }
    return 0;
}

