/*******************************************************************************
    > File Name: tran_file.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#include "../include/head.h"
#define FILENAME "file"

typedef struct {
    int dataLen;
    char buf[1000];
}train_t;

void sigFunc(int signum) {
    printf("%d is coming\n", signum);
}

int tranFile(int newFd) {
    signal(SIGPIPE, sigFunc);
    train_t train;
    int ret;
    train.dataLen = strlen(FILENAME);
    strcpy(train.buf, FILENAME);
    send(newFd, &train, 4 + train.dataLen, 0);

    struct stat buf;

    int fd = open(FILENAME, O_RDWR);
    fstat(fd, &buf);
    train.dataLen = sizeof(buf.st_size);
    memcpy(train.buf, &buf.st_size, train.dataLen);
    send(newFd, &train, 4 + train.dataLen, 0);

    ret = sendfile(newFd, fd, NULL, buf.st_size);
    printf("sendfile ret = %d\n", ret);
    ERROR_CHECK(ret, -1, "sendfile");

    return 0;
}
