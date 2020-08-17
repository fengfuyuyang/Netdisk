/*******************************************************************************
    > File Name: config.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-08
 ******************************************************************************/

#include "../include/head.h"

bool configInfo(pConfig_t config)
{
    FILE *fp;
    if (NULL == (fp = fopen("../conf/client.conf", "r"))) {
        /* perror("config"); */
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
            } 
            
            if (0 == strcmp(key, "port")) {
                strcpy(config->port, value);
            }

            bzero(key, sizeof(key));
        }
    }

    if (!strlen(config->ip) || !strlen(config->port)) {
        /* puts("Configuration error, please check the client.conf!"); */
        return false;
    } else {
        return true;
    }
}

