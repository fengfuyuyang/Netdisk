/*******************************************************************************
    > File Name: factory.h
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#ifndef __FACTORY_H__ 
#define __FACTORY_H__
#include "work_que.h"

typedef struct {
    char ip[17];
    int port;
    int threadnum;
    int capacity;
}Config_t, *pConfig_t;

typedef struct {
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    int threadNum;
    short startFlag;
}Factory_t, *pFactory_t;

bool configInfo(pConfig_t);

int factoryInit(pFactory_t, int, int);
int factoryStart(pFactory_t);

int tcpInit(int*, char*, int);
void setNonBlock(int);

int tranFile(int);
#endif

