/*******************************************************************************
    > File Name: config.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "../include/factory.h"

bool configInfo(pConfig_t config)
{
    FILE *fp;
    if (NULL == (fp = fopen("../conf/server.conf", "r"))) {
        return false;
    }
    char *p, *q;
    char buf[64] = {};
    char key[16] = {};
    char value[16] = {};

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
                strcpy(config->port, value);
            } else if (0 == strcmp(key, "threadnum")) {
                config->threadnum = atoi(value);
            }else if (0 == strcmp(key, "capacity")) {
                config->capacity = atoi(value);
            }
            bzero(key, sizeof(key));
        }
    }

    if (!strlen(config->ip) || !strlen(config->port) 
        || !config->threadnum || !config->capacity) {
        /* puts("Configuration error, please check the server.conf!"); */
        return false;
    } else {
        return true;
    }
}

