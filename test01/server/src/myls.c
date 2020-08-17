/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <time.h> */
/* #include <sys/stat.h> */
/* #include <fcntl.h> */
/* #include <sys/types.h> */
/* #include <unistd.h> */
/* #include <dirent.h> */
/* #include <pwd.h> */
/* #include <grp.h> */
#include "../include/head.h"

char *file_mode(const struct stat* file)
{
	char *ret = (char *)calloc(11, sizeof(char));
	char types[] = "xwrpcdb-ls";
	int mode = file->st_mode & S_IFMT;
	ret[0] = types[(mode >>= 12) / 2 + 3];
	mode = file->st_mode;
	int  pos = 9;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (mode & 1)
			{
				ret[pos--] = types[j];
			}
			else 
			{
				ret[pos--] = types[7];
			}
			mode >>= 1;
		}
	}
	return ret;
}

void file_mtime(const struct stat* file)
{
	struct tm* t = localtime(&file->st_mtime);
	printf("%2d月 %2d %02d:%02d ", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
}

void list_info(const char* filename , const struct stat* file)
{
	printf("%s %2ld %s %s %6ld ", file_mode(file), file->st_nlink, getpwuid(file->st_uid)->pw_name,getgrgid(file->st_gid)->gr_name, file->st_size);
	file_mtime(file);
	printf("%s\n",filename);
}

void list_dir(const char* pathname)
{
	DIR* ret_opendir = opendir(pathname);
	if(ret_opendir == NULL)
	{
		perror("opendir");
		return;
	}
	int ret_chdir = chdir(pathname);
	if(ret_chdir == -1)
	{
		perror("chdir");
		return;
	}
	struct dirent* ret_readdir = NULL;
	while((ret_readdir = readdir(ret_opendir)))
	{
		char* filename = ret_readdir->d_name;
		struct stat file_message = {};
		int ret_stat = lstat(filename, &file_message);
		if(ret_stat == -1)
		{
			printf("%s error!", filename);
		}
		else if(strcmp(filename,".") && strcmp(filename,".."))
		{
			list_info(filename, &file_message);
		}
	}
	closedir(ret_opendir);
}

int main(int  argc, const char** argv)
{
	if (argc != 2)
	{
		printf("error args\n");
	}
	list_dir(argv[1]);
	return 0;
}
