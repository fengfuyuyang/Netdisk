/*******************************************************************************
    > File Name: pwdCmd.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

void pwdCmd(int newFd, const char* homepath, const char* nowpath) {
    train_t train;
    strcpy(train.buf, nowpath + strlen(homepath));
    train.dataLen = strlen(train.buf);
    if (0 == train.dataLen) {
        strcpy(train.buf, "/");
        train.dataLen = 1;
    }
    send(newFd, &train, train.dataLen + 4, 0);
}

