/*******************************************************************************
    > File Name: work_que.h
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-19
 ******************************************************************************/

#ifndef __WORK_QUE_H__
#define __WORK_QUE_H__
#include "head.h"

typedef struct tag_node {
    int newFd;
    struct tag_node* pNext;
}Node_t, *pNode_t;

typedef struct {
    pNode_t queHead, queTail;
    int queCapacity;    //队列最大长度
    int queSize;        //队列当前长度
    pthread_mutex_t mutex;  //队列锁
}Que_t, *pQue_t;

void queInit(pQue_t, int);
void queInsert(pQue_t, pNode_t);
int queGet(pQue_t, pNode_t*);

#endif

