/*******************************************************************************
    > File Name: setNonBlock.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-15
 ******************************************************************************/

#include "factory.h"

void setNonBlock(int fd)
{
    int status;
    status = fcntl(fd, F_GETFL);
    status = status | O_NONBLOCK;
    fcntl(fd, F_SETFL, status);
}

