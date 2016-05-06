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

int readPath(char fileName[512], char line[512][1024]);
void seekFile(char dirPath[1024]);
void moveFile(char filePath[1024]);
char *generatePath(char *s1, char *s2, char *s3);

int count = 0;

int main(int argc, char* argv[])
{
	int i = 0;
	char line[512][1024];
	int length;

	if(argc != 2)
	{
		printf("Usage: executable file name plus  directory name\n");
		exit(EXIT_FAILURE);
	}

	printf("\n....................................begin copy....................................\n\n");
	length = readPath(argv[1], line);
	printf("************************************length = %d*************************************\n\n", length);
	printf("----------------------------------------------------------------------------------\n");
	for(i = 0; i < length; i++)
	{
		printf("                          %s\n", line[i]);
		seekFile((char *)line[i]);
	}
	printf("\n.....................................end copy.....................................\n\n");

	return 1;
}

int readPath(char fileName[512], char line[512][1024])
{
	FILE *fp;
	int index1 = 0;
	int index2 = 0;
	char ch;
	int i = 0;

	fp = fopen(fileName, "r");
	if(fp == NULL)
		ERR_EXIT("open file error");
	for(i = 0; i < 512; i++)
	{
		memset(line[index1], 0, 1024);
		index1++;
	}
	index1 = 0;
	while((ch = getc(fp)) != EOF)
	{
		if(ch != '\n')
		{
			line[index1][index2] = ch;
			index2++;
		}
		else
		{
			//ch = '\0';
			line[index1][index2-1] = '\0';
			printf("                         %d %s\n", index1, line[index1]);
			index1++;
			index2 = 0;
		}
	}	
	
	fclose(fp);
	return index1;
}

void seekFile(char dirPath[1024])
{
	printf("----------------------------------------------------------------------------------\n");
	DIR *dir = opendir(dirPath);
	if(dir == NULL)
	{
		printf("open %s error!\n", dirPath);
		exit(EXIT_FAILURE);
	}
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
			printf("%s\n", dirPath);
			char* rnewDir = generatePath(newDir, dirPath, de->d_name);
			seekFile(rnewDir);
		}	
		else
		{
			printf("                %s\n", dirPath);
			memset(filePath, 0, 1024);
			char* rfilePath = generatePath(filePath, dirPath, de->d_name);
			//strcpy(filePath, dirPath);
			//strcat(filePath, "/");
			//strcat(filePath, de->d_name);	
			moveFile(rfilePath);
		}
	}
	
	closedir(dir);	
	printf("----------------------------------------------------------------------------------\n");
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
	strcat(path, "./books/images/");
	strcat(path, fileName);
	printf("      %s\n", path);
		
	system(path);
}

char *generatePath(char *s1, char *s2, char *s3)
{
	strcpy(s1, s2);
	strcat(s1, "/");
	strcat(s1, s3);

	return s1;
}
