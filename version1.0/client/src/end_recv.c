/*******************************************************************************
    > File Name: end_recv.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-23
 ******************************************************************************/

#include "../include/head.h"

/* 用于成功无返回数据，失败返回错误信息 */
int endRcv(int socketFd) {
    int dataLen;
    char buf[512] = {0};

    int ret = recvCycle(socketFd, &dataLen, 4);
    if (-1 == ret) {
        return -1;
    }

    /* dataLen不为0，表示命令请求错误，接收返回的错误信息 */
    if (dataLen) {
        recvCycle(socketFd, buf, dataLen);
        puts(buf);
    }
    return 0;
}

