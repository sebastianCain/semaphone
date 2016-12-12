#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <string.h>

int main() {
  if (open("resource.txt", O_RDWR, 0) == -1) {
    printf("please create with './control -c' first\n");
    return 0;
  }

  int semd = semget(ftok("resource.txt", 5678), 1, 0);
  printf("semd: %d\n", semd);
  struct sembuf b;
  b.sem_op = -1;
  b.sem_num = 0;
  b.sem_flg = SEM_UNDO;
  
  semop(semd, &b, 1);
  
  int shmd = shmget(ftok("resource.txt", 1234), 100, 0);
  
  // faulty code after this comment
  
  int *shmaddr = shmat(shmd, 0, 0);  //size??
  int fd = open("resource.txt", O_RDWR | O_APPEND, 0666);
  char *line = malloc(100);
  lseek(fd, -(*shmaddr), SEEK_END);
  read(fd, line, *shmaddr);
  printf("Last line: %s\n", line);
  printf("Next line:");
  char *buff = malloc(100);
  fgets(buff, 100, stdin);
  write(fd, buff, 100);
  
  //b.sem_op = 1;
  //semop(semd, &b, 1);
  printf("%d\n", *shmaddr);
  return 0;
}
