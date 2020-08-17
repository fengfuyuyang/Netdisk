/*******************************************************************************
    > File Name: test.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "../include/factory.h"

int main()
{
    Config_t config = {};
    if (!configInfo(&config)) {
        puts("Configuration error, please check the server.conf!");
        return -1;
    }

#if 0
    printf("ip: %s\nport: %s\nthreadnum: %d\ncapacity: %d\n", config.ip, config.port,
           config.threadnum, config.capacity);
#endif

    int socketFd, ret;
    ret = tcpInit(&socketFd, config.ip, config.port);
    if (-1 == ret) {
        return -1;
    }

    int newFd;
    char buf[16];
    int epfd = epoll_create(1);
    struct epoll_event event, evs[3];
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    event.data.fd = socketFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, socketFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    int readyFdCount, i;
    while (1) {
        readyFdCount = epoll_wait(epfd, evs, 3, -1);
        for (i=0; i < readyFdCount; i++) {
            if (evs[i].data.fd == socketFd) {
                newFd = accept(socketFd, NULL, NULL);
                setNonBlock(newFd);// 设置newFd为非阻塞
                event.events = EPOLLIN | EPOLLET;// 监听读事件，边沿触发
                event.data.fd = newFd;
                ret = epoll_ctl(epfd,EPOLL_CTL_ADD, newFd, &event);
                ERROR_CHECK(ret, -1, "epoll_ctl");
            }

            if (evs[i].events == EPOLLIN && evs[i].data.fd == STDIN_FILENO) {
                bzero(buf,sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if (0 == ret) {
                    printf("byebye\n");
                    goto end;
                }
                /* 客户端接收数据大小为sizeof(buf) - 1 */
                /* bug:服务端的回车操作也会被客户端接收，客户端也出现换行 */
                send(newFd, buf, strlen(buf), 0); 
            }

            if (evs[i].data.fd == newFd) {
                while (1) {
                    bzero(buf,sizeof(buf));
                    ret = recv(newFd, buf, sizeof(buf) - 1, 0);

                    if (0 == ret) {
                        printf("byebye\n");
                        event.data.fd = newFd;
                        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, newFd, &event);//解除注册
                        ERROR_CHECK(ret, -1, "epoll_ctl");
                        close(newFd);
                        break;
                    } else if (-1 == ret) {
                        break;
                    } else {
                        printf("%s", buf);
                    }
                }

                /* printf("\n"); */
            }
        }
    }
end:
    close(socketFd);

    return 0;
}

