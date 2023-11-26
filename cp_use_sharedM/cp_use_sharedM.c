#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SHM_SIZE 1024
#define SEM_KEY 1234

union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
};

void sem_wait(int sem_id) {
        struct sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);
}

void sem_signal(int sem_id) {
        struct sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(sem_id, &sem_op, 1);
}

int main(int argc, char *argv[]) {
	key_t shm_key = ftok("shared_memory_key", 1);
	int shm_id = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0666);
	if (shm_id == -1) {
	       perror("shmget");
	       exit(EXIT_FAILURE);
	}

	key_t sem_key = ftok("semaphore_key", 1);
	int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
	if (sem_id == -1) {
	       perror("semget");
	       exit(EXIT_FAILURE);
	}

	union semun sem_init;
	sem_init.val = 1;
	semctl(sem_id, 0, SETVAL, sem_init);

        pid_t pid = fork();

	if (pid == -1) {
	         perror("fork");
	         exit(EXIT_FAILURE);
	}

	if (pid == 0) {
	char *shared_memory = shmat(shm_id, NULL, 0);
	FILE *source_file = fopen(argv[1], "r");
	if (source_file == NULL) {
	           perror("fopen");
	           exit(EXIT_FAILURE);
	 }

	sem_wait(sem_id);

	fread(shared_memory, sizeof(char), SHM_SIZE, source_file);
        sem_signal(sem_id);
        shmdt(shared_memory);
        fclose(source_file);
        exit(EXIT_SUCCESS);
        } else {
            wait(NULL); 
            char *shared_memory = shmat(shm_id, NULL, 0);
            FILE *destination_file = fopen(argv[2], "w");
            if (destination_file == NULL) {
															                perror("fopen");
                  exit(EXIT_FAILURE);
            }
            sem_wait(sem_id);
            fwrite(shared_memory, sizeof(char), SHM_SIZE, destination_file);
            sem_signal(sem_id);
            shmdt(shared_memory);
            fclose(destination_file);
																											        }
            shmctl(shm_id, IPC_RMID, NULL);
	    semctl(sem_id, 0, IPC_RMID);
	    return 0;
}
