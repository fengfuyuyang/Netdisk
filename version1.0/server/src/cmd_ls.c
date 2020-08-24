/*******************************************************************************
    > File Name: lsCmd.c
    > Author: fengfuyuyang
    > Mail: fengfuyuyang@outlook.com
    > Created Date: 2020-08-22
 ******************************************************************************/

#include "../include/factory.h"

char *file_mode(const struct stat* file) {
	char *ret = (char *)calloc(11, sizeof(char));
	char types[] = "xwrpcdb-ls";
	int mode = file->st_mode & S_IFMT;
	ret[0] = types[(mode >>= 12) / 2 + 3];
	mode = file->st_mode;
	int  pos = 9;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (mode & 1) {
				ret[pos--] = types[j];
			} else {
				ret[pos--] = types[7];
			}
			mode >>= 1;
		}
	}
	return ret;
}

void list_info(const char* filename , const struct stat* file, char* buf) {
	struct tm* t = localtime(&file->st_mtime);
	sprintf(buf, "%s %2ld %s %s %6ld %2d月 %2d %02d:%02d %s", 
           file_mode(file), 
           file->st_nlink, 
           getpwuid(file->st_uid)->pw_name, 
           getgrgid(file->st_gid)->gr_name, 
           file->st_size, 
           t->tm_mon + 1, 
           t->tm_mday, 
           t->tm_hour, 
           t->tm_min, 
           filename);
}

int lsCmd(int newFd, const char* homepath, char* pathname) {
    train_t train;
    if (0 == strcmp(pathname, "")) {
        strcpy(pathname, ".");
    }
	DIR* ret_opendir = opendir(pathname);

	if (ret_opendir == NULL) {
        sprintf(train.buf, "ls: cannot access '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
		/* perror("opendir"); */
        endFlag(newFd);
		return -1;
	}

    /* 降低越界访问，进入非用户自己的文件的可能性 */
    /* realpath(),第二个参数使用NULL，会自动malloc足够的空间来存放绝对路径，需要手动释放空间 */
    char * actualpath = realpath(pathname, NULL);
    if (strlen(actualpath) < strlen(homepath)) {
        sprintf(train.buf, "ls: cannot access '%s': No such file or directory", pathname);
        train.dataLen = strlen(train.buf);
        send(newFd, &train, train.dataLen + 4, 0);
        endFlag(newFd);
        free(actualpath);
        return -1;
    }
    free(actualpath);

    struct dirent* ret_readdir = NULL;

    while ((ret_readdir = readdir(ret_opendir))) {
        char* filename = ret_readdir->d_name;
        struct stat file_message = {};
        int ret_stat = lstat(filename, &file_message);

        if (ret_stat == -1) {
            printf("%s error!", filename);
        } else if(strcmp(filename,".") && strcmp(filename,"..")) {
            list_info(filename, &file_message, train.buf);
            train.dataLen = strlen(train.buf);
            send(newFd, &train, train.dataLen + 4, 0);
        }
    }
    endFlag(newFd);
    closedir(ret_opendir);

    return 0;
}

