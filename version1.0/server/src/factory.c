/*******************************************************************************
    > File Name: factory.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#include "../include/factory.h"

void* threadFunc(void* p) {
    pFactory_t pThreadInfo = (pFactory_t)p;
    pQue_t pq = &pThreadInfo->que;
    pNode_t pGet;

    int getTaskSuccess;


    while (1) {
        pthread_mutex_lock(&pq->mutex);

        if (!pq->queSize) {
            pthread_cond_wait(&pThreadInfo->cond, &pq->mutex);
        }

        getTaskSuccess = queGet(pq, &pGet); //拿任务
        pthread_mutex_unlock(&pq->mutex);


        printf("get job\n");


        if (!getTaskSuccess) {
            char usrname[20] = {0};
            char homepath[128] = {0};
            char nowpath[128] = {0};
            int ret;
            while (1) {
                 ret = verify_usr(pGet->newFd, usrname);
                 if (0 == ret) {
                     break;
                 }
            }
            printf("usr: %s\n", usrname);

            sprintf(homepath, "%s/usr/%s", getcwd(NULL, 0), usrname);
            chdir(homepath);
            puts(homepath);
            strcpy(nowpath, homepath);
            printf("%s login\n", usrname);

            while (1) {
                int ret;
                ret = cmdPoll(pGet->newFd, homepath, nowpath);
                if (-1 == ret) {
                    printf("%s offline\n", usrname);
                    break;
                }
            }
            free(pGet);
            pGet = NULL;
        }
    }
}

int factoryInit(pFactory_t p, int threadNum, int capacity) {
    queInit(&p->que, capacity);
    pthread_cond_init(&p->cond, NULL);
    p->pthid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    p->threadNum = threadNum;
    p->startFlag = 0;

    return 0;
}

int factoryStart(pFactory_t p) {
    if (!p->startFlag) {
        int i;
        for (i = 0; i < p->threadNum; ++i) {
            pthread_create(p->pthid + i, NULL, threadFunc, p);
        }
        p->startFlag = 1;
    }

    return 0;
}

