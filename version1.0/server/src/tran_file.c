/*******************************************************************************
    > File Name: tran_file.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#include "../include/factory.h"
/* #define FILENAME "file" */


void sigFunc(int signum) {
    printf("%d is coming\n", signum);
}

int tranFile(int newFd) {
    char pathname[] = "path/file";
    char* filename = basename(pathname);
    signal(SIGPIPE, sigFunc);
    train_t train;
    int ret;
    train.dataLen = strlen(filename);
    strcpy(train.buf, filename);
    send(newFd, &train, 4 + train.dataLen, 0);

    struct stat buf;

    int fd = open(pathname, O_RDWR);
    fstat(fd, &buf);
    train.dataLen = sizeof(buf.st_size);
    memcpy(train.buf, &buf.st_size, train.dataLen);
    send(newFd, &train, 4 + train.dataLen, 0);

    ret = sendfile(newFd, fd, NULL, buf.st_size);
    printf("sendfile ret = %d\n", ret);
    ERROR_CHECK(ret, -1, "sendfile");

    return 0;
}
