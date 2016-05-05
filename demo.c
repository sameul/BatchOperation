#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void seekFile(char dirPath[1024]);
void moveFile(char filePath[1024]);
char *generatePath(char *s1, char *s2, char *s3);


int count = 0;

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: executable file name plus  directory name\n");
		exit(EXIT_FAILURE);
	}

	printf("\n....................................begin copy....................................\n\n");
	seekFile(argv[1]);
	printf("\n.....................................end copy.....................................\n\n");

	return 1;
}

void seekFile(char dirPath[1024])
{
	DIR *dir = opendir(dirPath);
	struct dirent *de;
	char newDir[1024];
	char filePath[1024];

	while((de = readdir(dir)) != NULL)
	{
		memset(newDir, 0, 1024);
		if(strncmp(de->d_name, ".", 1) == 0)
			continue;
		if(de->d_type == DT_DIR)
		{
			char* rnewDir = generatePath(newDir, dirPath, de->d_name);
			seekFile(rnewDir);
		}	
		else
		{
			memset(filePath, 0, 1024);
			char* rfilePath = generatePath(filePath, dirPath, de->d_name);
			//strcpy(filePath, dirPath);
			//strcat(filePath, "/");
			//strcat(filePath, de->d_name);	
			moveFile(rfilePath);
		}
	}
	
	closedir(dir);	
}

void moveFile(char filePath[1024])
{
	char path[1024];
	char fileName[512];

	memset(path, 0, 1024);
	memset(fileName, 0, 512);
	strcpy(path, "cp ");
	strcat(path, filePath);
	strcat(path, " ");
	sprintf(fileName, "%d.jpg", count++);	
	strcat(path, "./books1/images/");
	strcat(path, fileName);
	printf("%s\n", path);
		
	system(path);
}

char *generatePath(char *s1, char *s2, char *s3)
{
	strcpy(s1, s2);
	strcat(s1, "/");
	strcat(s1, s3);

	return s1;
}
