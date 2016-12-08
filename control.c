#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char *argv[]){

  int sem;
  int shmem;
  int file;
  int semkey = ftok("control.c" , 22);
  int shmemkey = ftok("control.c", 40);
  int sh;
  int sc;

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    sem = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0644);
    printf("semaphore created: %d\n", sem);

    shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | 0644);
    printf("shared memory created: %d\n", shmem);

    file = open("story.txt", O_TRUNC | O_CREAT | O_RDWR, 0644);
    printf("file created: %d\n", file);

    union semun su;
    su.val = 1;//atoi(argv[2]);

    //set sem value
    sc = semctl(sem, 0, SETVAL, su);
    printf("value set: %d\n", sc);
  }

  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    sem = semget(key, 1, 0);

    //get value of sem
    sc = semctl(sem, 0, GETVAL);
    printf("semaphore value: %d\n",sc);
  }

  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    sem = semget(semkey, 1, 0);
    shmem = shmget(shmemkey, sizeof(int), 0);
    file = open("story.txt", O_RDONLY);

    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char buf[size]; // = (char*)malloc(sizeof(char*));

    read(file,buf,sizeof(buf));
    printf("Story:\n\n%s\n\n",buf);
    

    //remove sem
    sc = semctl(sem, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);

    sh = shmctl(shmem, 0, IPC_RMID);
    printf("shared memory removed: %d\n", sh);
  }

  return 0;

}
