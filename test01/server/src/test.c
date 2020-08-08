/*******************************************************************************
    > File Name: test.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "factory.h"

int main()
{
    Config_t config = {};
    configInfo(&config);
    printf("ip: %s\nport: %s\nthreadnum: %d\ncapacity: %d\n", config.ip, config.port,
           config.threadnum, config.capacity);
    return 0;
}

