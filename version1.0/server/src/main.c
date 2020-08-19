/*******************************************************************************
    > File Name: main.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#include "../include/factory.h"

int main()
{
    Config_t config = {};
    if (!configInfo(&config)) {
        puts("Configuration error, please check the server.conf!");
        return -1;
    }

    Factory_t threadInfo;
    factoryInit(&threadInfo, config.threadnum, config.capacity);
    factoryStart(&threadInfo);

    int socketFd;
    tcpInit(&socketFd, config.ip, config.port);
    int newFd;

    pQue_t pq = &threadInfo.que;
    pNode_t pNew;

    while (1) {
        newFd = accept(socketFd, NULL, NULL);
        pNew = (pNode_t)calloc(1, sizeof(Node_t));
        pNew->newFd = newFd;
        pthread_mutex_lock(&pq->mutex);
        queInsert(pq, pNew);        //放任务
        pthread_mutex_unlock(&pq->mutex);
        pthread_cond_signal(&threadInfo.cond);      //唤醒子线程
    }
    return 0;
}

