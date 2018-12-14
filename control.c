#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

union semun{
	int val;
	struct semid_ds * buf;
	unsigned short * array;
  struct seminfo * _buf;
};

//makes shared memory, semaphore, file
int create(){
  int fd = open("story", O_CREAT| O_TRUNC, 0666);
  int key = ftok("control.c", 'R');
  int sem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
  int shm = shmget(key, 1000, IPC_CREAT | IPC_EXCL | 0644 );

  //set value to 1
  union semun a;
  a.val = 1;
  semctl(sem,0,SETVAL,a);
}

//view story
int view(){
  int fd = open("story", O_RDONLY);
  char * story = malloc (1000);
  read(fd, story, 1000);
  printf("%s\n",story);
  close(fd);
}

int delete(){
  int key = ftok("control.c", 'R');
  int sem = semget(key,1,0644);
  struct sembuf b;
  b.sem_op = -1;
  b.sem_num = 0;
  b.sem_flg = SEM_UNDO;
  semop(sem, &b ,1);

  view();
  //remove story
  remove("story");
  //remove semaphore
  semctl(sem,0,IPC_RMID,0);

  //remove memory
  int mem = shmget(key,1000,0664);
  shmctl(mem,0,IPC_RMID);

}

int main(int c, char * ans[]){
  if(c ==1){
  printf("Enter command\n");
  exit(1);}

  if(strcmp(ans[1],"-c")==0){
    create();
  }
  else if(strcmp(ans[1],"-r")==0){
      delete();
    }
  else if(strcmp(ans[1],"-v")==0){
        view();
    }

}
