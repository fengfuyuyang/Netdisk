/*******************************************************************************
    > File Name: login.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-21
 ******************************************************************************/

#define _XOPEN_SOURCE
#include "../include/head.h"

int login(int socketFd, char* usrname) {
    train_t train = {};
    int dataLen;
    printf("Please enter usrname: ");
    scanf("%s", usrname);
    strcpy(train.buf, usrname);
    train.dataLen = strlen(train.buf);

    send(socketFd, &train, train.dataLen + 4, 0); //发送用户名
    bzero(&train, sizeof(train));
    recvCycle(socketFd, &dataLen, 4);
    if (dataLen) {
        recvCycle(socketFd, train.buf, dataLen);
        puts(train.buf);
        return -1;
    }

    bzero(&train, sizeof(train));
    char salt[16] = {0};
    recvCycle(socketFd, &dataLen, 4);
    recvCycle(socketFd, salt, dataLen);

    char* password = getpass("Please enter password: ");
    strcpy(train.buf, crypt(password, salt));
    train.dataLen = strlen(train.buf);
    send(socketFd, &train, train.dataLen + 4, 0); //发送密码密文

    bzero(&train, sizeof(train));
    recvCycle(socketFd, &dataLen, 4);
    if (dataLen) {
        recvCycle(socketFd, train.buf, dataLen);
        puts(train.buf);
        return -1;
    }
    return 0;
}
