/*******************************************************************************
    > File Name: factory.h
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#ifndef __FACTORY_H__ 
#define __FACTORY_H__
#include "head.h"
typedef struct Config {
    char ip[17];
    char port[6];
    int threadnum;
    int capacity;
}Config_t, *pConfig_t;

bool configInfo(pConfig_t);
int tcpInit(int*, char*, char*);
void setNonBlock(int);
#endif

