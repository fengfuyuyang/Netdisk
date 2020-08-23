/*******************************************************************************
    > File Name: epoll_op.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-23
 ******************************************************************************/

#include "../include/head.h"

int epollInAdd(int epfd, int fd) {
    struct epoll_event event;
    event.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

