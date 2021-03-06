/*******************************************************************************
    > File Name: cmd_gets.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-24
 ******************************************************************************/

#include "../include/head.h"

int putsCmd(int socketFd, char* pathname) {
    printf("send file: %s\n", pathname);
    train_t train;
    int ret;
    
    char* filename = basename(pathname);
    
    train.dataLen = strlen(filename);
    strcpy(train.buf, filename);
    send(socketFd, &train, train.dataLen + 4, 0);

    struct stat buf;
    int fd = open(pathname, O_RDWR);
    fstat(fd, &buf);
    train.dataLen = sizeof(buf.st_size);
    memcpy(train.buf, &buf.st_size, train.dataLen);
    send(socketFd, &train, train.dataLen + 4, 0);

    if (buf.st_size < (100 << 20)) {
        printf("send small file\n");
        while ((train.dataLen = read(fd, train.buf, sizeof(train.buf)))) {
            ret = send(socketFd, &train, 4 + train.dataLen, 0);
            ERROR_CHECK(ret, -1, "send_small_train");
        }
        send(socketFd, &train, 4, 0);
    } else {
        printf("send big file\n");
        char* pMap = (char*)mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        ret = send(socketFd, pMap, buf.st_size, 0);
        ERROR_CHECK(ret, -1, "send_mmap");
    }
    close(fd);
    return 0;
}

