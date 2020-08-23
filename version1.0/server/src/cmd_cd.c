/*******************************************************************************
    > File Name: cdCmd.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

int cdCmd(int newFd, const char* homepath, char* nowpath, const char* pathname) {
    train_t train;
    char buf[512] = {0};
    if (0 == strcmp(pathname, "") || 0 == strcmp(pathname, "~")) {
        chdir(homepath);
        
        /* 更新当前路径 */
        strcpy(nowpath, homepath);
    } else {
        if ('~' == pathname[0]) {
            sprintf(buf, "%s/%s", nowpath, pathname + 1);
        } else {
            sprintf(buf, "%s/%s", nowpath, pathname);
        }
        /* DIR * ret = opendir(buf); */
        if (!access(buf, F_OK)) {
            sprintf(train.buf, "cd: %s: No such file or directory", pathname);
            train.dataLen = strlen(train.buf);
            send(newFd, &train, 4 + train.dataLen, 0);
            return 0;
        }

        struct stat dir_stat;
        if (stat(buf, &dir_stat) < 0) {
            sprintf(train.buf, "cd: %s: No such file or directory", pathname);
            train.dataLen = strlen(train.buf);
            send(newFd, &train, 4 + train.dataLen, 0);
            return -1;
        }

        /* 降低越界访问，进入非用户自己的文件的可能性 */
        /* realpath(),第二个参数使用NULL，会自动malloc足够的空间来存放绝对路径，需要手动释放空间 */
        char * actualpath = realpath(pathname, NULL);
        if (strlen(actualpath) < strlen(homepath)) {
            sprintf(train.buf, "cd: %s: No such file or directory", pathname);
            train.dataLen = strlen(train.buf);
            send(newFd, &train, train.dataLen + 4, 0);
            free(actualpath);
            actualpath = NULL;
            return -1;
        }
        free(actualpath);
        actualpath = NULL;

        /* pathname为普通文件,直接退出 */
        if (S_ISREG(dir_stat.st_mode)) {
            sprintf(train.buf, "cd: %s: Not a directory", pathname);
            train.dataLen = strlen(train.buf);
            send(newFd, &train, 4 + train.dataLen, 0);
            return -1;
        } else if (S_ISDIR(dir_stat.st_mode)) {
            chdir(buf);

            /* 更新当前目录路径 */
            strcpy(nowpath, getcwd(NULL, 0));
        }
    }
    endFlag(newFd);
    /* printf("nowpath: %s\n", nowpath); */

    return 0;
}

