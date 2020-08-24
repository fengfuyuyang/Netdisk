/*******************************************************************************
    > File Name: cmd_poll.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

int cmdPoll(int newFd, const char* homepath, char* nowpath) {

    train_t train;
    int dataLen;
    char buf[128] = {0};
    char cmd[8] = {0};
    char tmp[128] = {0};

    int ret = recvCycle(newFd, &dataLen, 4);

    if (-1 == ret) {
        return -1;
    }
    recvCycle(newFd, buf, dataLen);
    sscanf(buf, "%s %s", cmd, tmp);

    /* 校正当前所在目录, 避免多客户端连接时目录混乱 */
    chdir(nowpath);
#if 1
    printf("nowpath: %s\n", nowpath);
#endif

    if (0 == strcmp(cmd, "cd")) {
        cdCmd(newFd, homepath, nowpath, tmp);

        /* 给客户端发送更新后的当前目录 */
        pwdCmd(newFd, homepath, nowpath);
    } else if (0 == strcmp(cmd, "ls")) {
        lsCmd(newFd, homepath, tmp);
    } else if (0 == strcmp(cmd, "puts")) {
        /* putsCmd(newFd, tmp); */
    } else if (0 == strcmp(cmd, "gets")) {
        /* getsCmd(newFd, tmp); */
    } else if (0 == strcmp(cmd, "mkdir")) {
        mkdirCmd(newFd, tmp);
    } else if (0 == strcmp(cmd, "rm") || 0 == strcmp(cmd, "remove")) {

        /* removeCmd()中包含递归调用，可能多次发送数据给客户端，
         * 需要发送结束标志给客户端 */
        if (0 == removeCmd(newFd, homepath, tmp)) {
            endFlag(newFd);
        }
    } else if (0 == strcmp(cmd, "pwd")) {
        pwdCmd(newFd, homepath, nowpath);
    } else {
        sprintf(train.buf, "%s: command not found", cmd);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
    }
    return 0;
}

