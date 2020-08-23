/*******************************************************************************
    > File Name: endFlag.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-23
 ******************************************************************************/

/* 给客户端发送接收数据结束标志 */

#include "../include/factory.h"

void endFlag(int newFd) {
    int flag = 0;
    send(newFd, &flag, 4, 0);
}
