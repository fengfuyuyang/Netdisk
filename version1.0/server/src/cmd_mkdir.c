/*******************************************************************************
    > File Name: cmd_mkdir.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

int mkdirCmd(int newFd, const char* pathname) {
    train_t train;
    int ret;
    ret = mkdir(pathname, 0777);
    if (-1 == ret) {
        sprintf(train.buf, "mkdir: cannot create directory ‘%s’: No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        return -1;
    }
    int flag = 0;
    send(newFd, &flag, 4, 0);

    return 0;
}


