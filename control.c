#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>


union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
			      (Linux-specific) */
};

int main(int argc, char *argv[]){

  int sem;
  int shmem;
  int file;
  int semkey = ftok("control.c" , 22);
  int shmemkey = ftok("control.c", 40);
  int sh;
  int sc;
  int *size;

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    //SEMAPHORE
    sem = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0644);
    printf("semaphore created: %d\n", sem);
    union semun su;
    su.val = 1;
    sc = semctl(sem, 0, SETVAL, su);
    printf("value set: %d\n", sc);

    //SHARED MEMORY
    shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    if (shmem == -1) {
      printf("Shemem error: %s\n",strerror(errno));
    }
    size = shmat(shmem,0,0);
    *size = 0;

    //CREATE FILE
    file = open("story.txt", O_CREAT | O_TRUNC | O_RDWR | O_EXCL, 0644);
  }

  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    file = open("story.txt",O_RDONLY);
    int s = lseek(file,0,SEEK_END);
    lseek(file,-1*s,SEEK_CUR);
    char buf[s];
    read(file,buf,sizeof(buf));
    printf("Story:\n%s",buf);
  }

  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    sem = semget(semkey, 1, 0);
    shmem = shmget(shmemkey, sizeof(int), 0);
    
    file = open("story.txt", O_RDONLY);
    int s = lseek(file,0,SEEK_END);
    lseek(file,-1*s,SEEK_CUR);
    char buf[s];
    read(file,buf,sizeof(buf));
    printf("Story:\n\n%s\n\n",buf);
    close(file);
    
    //REMOVE SEMAPHORE
    sc = semctl(sem, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);

    //REMOVE SHARED MEMORY
    sh = shmctl(shmem, IPC_RMID, NULL);
    printf("shared memory removed: %d\n", sh);
  }

  return 0;

}
