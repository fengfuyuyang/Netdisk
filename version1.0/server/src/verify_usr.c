/*******************************************************************************
    > File Name: verify_usr.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-21
 ******************************************************************************/

#include "../include/factory.h"

void get_salt(char* salt, char* passwd) {
    int i, j;
    for (i = 0, j = 0; passwd[i] && j != 3; ++i) {
        if ('$' == passwd[i]) {
            ++j;
        } 
    }
    strncpy(salt, passwd, i - 1);
}

int verify_usr(int newFd, char* usrname) {
    train_t train;
    int dataLen;
    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd, usrname, dataLen);

    struct spwd *sp;
    char salt[512] = {0};

    if (NULL == (sp = getspnam(usrname))) {
        train.dataLen = 0;
        send(newFd, &train, 4, 0);
        return -1;
    }
    get_salt(salt, sp->sp_pwdp);

    train.dataLen = strlen(salt);
    strcpy(train.buf, salt);
    send(newFd, &train, 4 + train.dataLen, 0);

    char passwd[512] = {0};

    recvCycle(newFd, &dataLen, 4);
    recvCycle(newFd, passwd, dataLen);

    printf("usrname: %s\n", usrname);

    if (0 == strcmp(sp->sp_pwdp, passwd)) {
        train.dataLen = 1;
        send(newFd, &train, 4, 0);
        return 0;
    } else {
        dataLen = 0;
        send(newFd, &dataLen, 4, 0);
        return -1;
    }
}

