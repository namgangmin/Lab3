

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MESSAGE_SIZE 256
#define MAX_USERNAME_SIZE 50

struct ChatMessage{
	long messageType;
	char username[MAX_USERNAME_SIZE];
	char message[MAX_MESSAGE_SIZE];
};

void sendMessage(int msqid, struct ChatMessage *message){
	if (msgsnd(msqid, message, sizeof(struct ChatMessage) - sizeof(long),0) == -1){
		perror("Error sending message");
		exit(EXIT_FAILURE);
	}
}

void receiveMessages(int msqid){
	struct ChatMessage message;

	while(1) {
		if(msgrcv(msqid, &message, sizeof(struct ChatMessage) - sizeof(long),1,0) == -1) {
			perror("Error receiving message");
			exit(EXIT_FAILURE);
		}

		printf("%s: %s\n", message.username, message.message);

		if (message.messageType == 2){
			break;
		}
	}
}

int main(){

	key_t key;
	int msqid;

	if ((key = ftok("/tmp", 'A')) == -1){
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	if((msqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
			perror("msgget");
			exit(EXIT_FAILURE);
	}

	printf("Chat program started. Type 'exit' to quit.\n");

	pid_t pid = fork();

	if(pid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		receiveMessages(msqid);
		exit(EXIT_SUCCESS);
	}
	else{
		struct ChatMessage message;
		message.messageType = 1;

		while(1){
			printf("Enter your username: " );
			scanf("%s", message.username);

			if (strcmp(message.username, "exit") == 0){
				message.messageType = 2;
				sendMessage(msqid, &message);
				break;
			}

			printf("You: ");
			scanf(" %s", message.message);

			sendMessage(msqid, &message);

			if(strcmp(message.message, "exit") == 0){
				message.messageType = 2;
				sendMessage(msqid, &message);
				break;
			}
		}

		wait(NULL);

		if(msgctl(msqid, IPC_RMID, NULL) == -1){
			perror("msgctl");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

