/*******************************************************************************
    > File Name: cmd_puts.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-24
 ******************************************************************************/

#include "../include/factory.h"

int putsCmd(int newFd) {
    /* train_t train; */
    int fd, ret;
    int dataLen;
    char buf[1000] = {0};

    /* 接收文件名 */
    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd, buf, dataLen);
    char* filename = basename(buf);

    fd = open(filename, O_CREAT | O_RDWR, 0666);
    ERROR_CHECK(fd, -1, "open");

    off_t fileSize;
    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd, &fileSize, dataLen);

    /* 文件大小小于100M, 使用小火车 */
    if (fileSize < (100 << 20)) {
        while (1) {
            recvCycle(newFd, &dataLen, 4);
            if (dataLen > 0) {
                ret = recvCycle(newFd, buf, dataLen);
                if (-1 == ret) {
                    printf("Client disconnected\n");
                    break;
                }
                write(fd, buf, dataLen);
            } else {
                break;
            }
        }
    } else {

        /* 文件大小大于100M，使用mmap传输文件 */
        ftruncate(fd, fileSize);
        char* pMap = (char*)mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        ERROR_CHECK(pMap, (char*)-1, "mmap");
        recvCycle(newFd, pMap, fileSize);
        munmap(pMap, fileSize);
    }
    close(fd);
    return 0;
}

