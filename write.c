#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main() {
  int sem;
  int shmem;
  int file;
  int semkey = ftok("control.c" , 22);
  int shmemkey = ftok("control.c", 40);
  int sh;
  int sc;
  int *size;
  char in[1000];
  //char *in;

  
  sem = semget(semkey,1,0);
  shmem = shmget(shmemkey,sizeof(int),IPC_CREAT | 0644);
  file = open("story.txt",O_RDWR);

  
  int f = lseek(file,0,SEEK_END);
  lseek(file,-1*f,SEEK_CUR);
  
  char buf[f];
  
  //I don't know
  if (*size != -1) {
    lseek(file,0,*size);
    read(file,buf,sizeof(buf));
    printf("Last line: %s\n\n",buf);
  }
  else {
    printf("attach\n");
    size = shmat(shmem,0,0);
    *size = 0;
  }
  

  printf("Enter next line:\n");
  fgets(in,sizeof(in),stdin);

  char* line = strstr(in,"\n");
  *line = 0;

  int i = 0;  //how many characters
  while (in[i]) {
    printf("%c > ",in[i]);
    i++;
  }
  
  lseek(file,0,SEEK_END);

  write(file,in,i);
  
  //*size += sizeof(in);
  
}
