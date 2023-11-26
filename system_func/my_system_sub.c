

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int my_system(const char *command){
	pid_t pid;
	int stat;

	if((pid = fork()) == -1) {
		perror("fork");
		return -1;
	}
	if (pid == 0){
		execl("/bin/sh","sh","-c",command, (char *)NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}
	else{
		if(waitpid(pid, &stat, 0) == -1) {
			perror("waitpid");
			return -1;
		}

		return stat;
	}
}

int main(int argc, char *argv[]){
	if (argc < 2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int command_length = 0;
	for(int i = 1; i < argc; ++i){
		command_length += snprintf(NULL, 0, "%s ", argv[i]);
	}

	char *command_str = (char *)malloc(command_length + 1);
	if(command_str == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	command_length = 0;
	for(int i = 1; i < argc; ++i){
		command_length += snprintf(command_str + command_length, command_length + 1,
				"%s ", argv[i]);
	}

	command_str[command_length - 1] = '\0';

	int result = my_system(command_str);

	printf("Command exited with stat %d\n",result);

	free(command_str);
	
	return 0;
}
