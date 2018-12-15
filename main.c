#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

int fd = open("story", O_RDWR | O_APPEND);
int key = ftok("control.c", 'R');
int sem = semget(key, 1, 0);
int shm = shmget(key, 1000, 0);

struct sembuf o;
o.sem_op = -1;
o.sem_num = 0;
o.sem_flg = SEM_UNDO;

char * line = malloc(1000);
line= shmat(sem,0,0);
printf("Displaying last line: %s\n",line);
printf("Input a line: \n");
char * newline = malloc(1000);
scanf("%s", newline);
strcpy(line, newline);
shmdt(line);

write(fd, newline, strlen(newline));
close(fd);
o.sem_op=1;
semop(sem,&o,1);

return 1;

}
