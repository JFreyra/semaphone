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

//http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/shm/shmat.html



int main() {

  //use semaphores to block
  //need to include in this file

  int shmem;
  int file;
  int shmemkey = ftok("control.c", 40);
  char in[1000];
  
  
  printf("NUUUU\n");

  shmem = shmget(shmemkey, sizeof(int), 0644);
  
  if (shmem == -1) {
    printf("Shemem error: %s\n",strerror(errno));
  }

  int *size = shmat(shmem,NULL,SHM_RDONLY);

  if (*size == -1) {
    printf("Shmat error: %s\n",strerror(errno));  
  }

  printf("size: %d\n",*size);
  

  file = open("story.txt",O_RDWR);

  char buf[500];

  printf("wsgusdg\n");
  
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
