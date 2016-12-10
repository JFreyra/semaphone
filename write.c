#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>



int main() {

  //use semaphores to block
  //need to include in this file

  int shmem, sem;
  int file;
  int shmemkey = ftok("control.c", 40);
  int semkey = ftok("control.c",22);
  char in[1000];
  
  //SEMAPHORE SETUP
  sem = semget(semkey, 1, 0);
  
  if (sem == -1) {
    printf("Sem error: %s\n",strerror(errno));
  }
  
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = 0;
  
  //SHARED MEMORY SETUP
  shmem = shmget(shmemkey, sizeof(int), 0644);
  
  if (shmem == -1) {
    printf("Shemem error: %s\n",strerror(errno));
  }

  int *size = shmat(shmem,NULL,0);

  if (*size == -1) {
    printf("Shmat error: %s\n",strerror(errno));  
  }


  //FILE READING (LAST LINE)
  file = open("story.txt",O_RDWR | O_APPEND);

  char buf[500];

  int sen = lseek(file,-(*size),SEEK_END);

  read(file,buf,sen);

  printf("Last line: %s\n\n",buf);

  //INPUT
  printf("Enter next line:\n");
  fgets(in,sizeof(in),stdin);

  char* line = strstr(in,"\n");
  line++;
  *line = 0;
  
  //WRITE TO FILE AND STORE LAST LINE
  write(file,in,strlen(in));
  *size = strlen(in); 
  
  close(file);
  shmdt(size);
  
}
