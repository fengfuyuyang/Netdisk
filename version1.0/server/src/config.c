/*******************************************************************************
    > File Name: config.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "../include/factory.h"

bool configInfo(pConfig_t config) {
    /* 读取服务器ip，port, threadnum, capacity 的配置信息 */
    FILE *fp;
    if (NULL == (fp = fopen("../conf/server.conf", "r"))) {
        return false;
    }
    char *p, *q;
    char buf[64] = {};
    char key[16] = {};
    char value[16] = {};

    /* config->threadnum = 5;      //设置默认线程数 */
    /* config->capacity = 10;      //设置默认队列长度最大值 */

    while (fgets(buf, 50, fp)) {
        p = strchr(buf, '=');
        q = strchr(buf, '\n');

        if (p && q) {
            *q = '\0';
            strncpy(key, buf, p - buf);
            strcpy(value, p + 1);

            if (0 == strcmp(key, "ip")) {
                strcpy(config->ip, value);
            } else if (0 == strcmp(key, "port")) {
                config->port = atoi(value);
                /* strcpy(config->port, value); */
            } else if (0 == strcmp(key, "threadnum")) {
                config->threadnum = atoi(value);
            }else if (0 == strcmp(key, "capacity")) {
                config->capacity = atoi(value);
            }
            bzero(key, sizeof(key));
        }
    }
    fclose(fp);

    /* 检查配置信息是否完整 */
    if (!strlen(config->ip) || !config->port 
        || !config->threadnum || !config->capacity) {
        /* puts("Configuration error, please check the server.conf!"); */
        return false;
    } else {
        return true;
    }
}

