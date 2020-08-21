/*******************************************************************************
    > File Name: login.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-21
 ******************************************************************************/

#define _XOPEN_SOURCE
#include "../include/head.h"
int login(int fd) {
    train_t train = {};
    int dataLen;
    char username[20] = {0};
    printf("Please enter usrname: ");
    scanf("%s", username);
    strcpy(train.buf, username);
    train.dataLen = strlen(train.buf);

    send(fd, &train, train.dataLen + 4, 0); //发送用户名

    recvCycle(fd, &dataLen, 4);
    if (0 == dataLen) {
        puts("username error");
        return -1;
    }

    bzero(&train, sizeof(train));
    char salt[16] = {0};
    recvCycle(fd, salt, dataLen);

    char* password = getpass("Please enter password: ");
    strcpy(train.buf, crypt(password, salt));
    train.dataLen = strlen(train.buf);
    send(fd, &train, train.dataLen + 4, 0); //发送密码密文

    recvCycle(fd, &dataLen, 4);
    if (0 == dataLen) {
        puts("password error!");
        return -1;
    }
    return 0;
}
