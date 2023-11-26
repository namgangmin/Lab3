

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

int ls_Command (char *argv)
{
	struct dirent *dp;
	DIR *dir;
	struct stat fstat;

	int i = 0;
	int count = 0;
	char *dir_name[255];

	memset(dir_name, '\0', sizeof(dir_name));
	memset(&dp, '\0', sizeof(dp));
	memset(&fstat, '\0', sizeof(fstat));

	if ((dir = opendir(argv)) < 0)
	{
		perror("opendir");
		return 1;
	}

	chdir(argv);

	printf("\n");
	printf("디렉토리 : %s \n", argv);

	while((dp = readdir(dir)) != NULL)
	{
		lstat(dp->d_name, &fstat);

		if (S_ISDIR(fstat.st_mode) == 1)
		{
			if(!strcmp(dp->d_name,".") || !strcmp(dp->d_name, ".."))
			{
				;
			}
			else
			{
				dir_name[count] = dp->d_name;
				count = count + 1;
			}
		}
		printf("%s\t", dp->d_name);
	}
	for(i = 0; i < count; i++)
	{
		ls_Command(dir_name[i]);
	}
	printf("\n");
	closedir(dir);
	chdir("..");
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr, "Usage : file_dir dirname.\n");
		return 1;
	}
	
	ls_Command(argv[1]);

	return 0;
}


