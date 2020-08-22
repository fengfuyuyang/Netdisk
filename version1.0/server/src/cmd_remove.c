/*******************************************************************************
    > File Name: cmd_remove.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

int removeCmd(int newFd, const char* homepath, const char* pathname) {
    train_t train;
    char dir_name[1024] = {0};
    DIR *ret_opendir;
    struct dirent *dp;
    struct stat dir_stat;

    if (0 == strcmp(pathname, ".") || 0 == strcmp(pathname, "..")) {
        sprintf(train.buf, "rm: refusing to remove '.' or '..' directory: skipping '%s'", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        return -1;
    }

    /* 若目录不存在，直接返回 */
    if (access(pathname, F_OK)) {
        sprintf(train.buf, "rm: cannot remove '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        return -1;
    }

    /* 降低越界访问，错误删除非用户自己的文件的可能性 */
    /* realpath(),第二个参数使用NULL，会自动malloc足够的空间来存放绝对路径，需要手动释放空间 */
    char * actualpath = realpath(pathname, NULL);
    if (strlen(actualpath) <= strlen(homepath)) {
        sprintf(train.buf, "rm: cannot remove '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        free(actualpath);
        return -1;
    }
    free(actualpath);

    if (stat(pathname, &dir_stat) < 0) {
        /* perror("stat"); */
        sprintf(train.buf, "rm: cannot remove '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        return -1;
    }

    if (S_ISREG(dir_stat.st_mode)) {
        remove(pathname);
    } else if (S_ISDIR(dir_stat.st_mode)) {
        ret_opendir = opendir(pathname);

        while ((dp = readdir(ret_opendir)) != NULL) {
            if ((0 == strcmp(".", dp->d_name)) 
                || (0 == strcmp("..", dp->d_name))) {
                continue;
            }

            sprintf(dir_name, "%s/%s", pathname, dp->d_name);
            removeCmd(newFd, homepath, dir_name);
        }
        closedir(ret_opendir);

        rmdir(pathname);
    } else {
        sprintf(train.buf, "rm: cannot remove '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
    }
    int endflag = 0;
    send(newFd, &endflag, 4, 0);
    return 0;
}

