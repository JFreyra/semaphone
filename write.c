#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

//http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html



int main() {

  //use semaphores to block
  //need to include in this file

  int shmem;
  int file;
  int shmemkey = ftok("control.c", 40);
  char in[1000];
  
  
  printf("NUUUU\n");

  shmem = shmget(shmemkey, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  int *size = shmat(shmem,0,0);
  
  

  file = open("story.txt",O_RDWR);
  
  char buf[*size];
  
  lseek(file,-(*size),SEEK_END);

  read(file,buf,sizeof(buf));

  printf("Last line: %s\n\n",buf);


  printf("Enter next line:\n");
  fgets(in,sizeof(in),stdin);

  char* line = strstr(in,"\n");
  *line = 0;

  int i = 0;  //how many characters
  while (in[i]) {
    //printf("%c > ",in[i]);
    i++;
  }
  
  lseek(file,0,SEEK_END);

  write(file,in,i);
  
  *size += sizeof(in);

  close(file);

  shmdt(size);
  
}
