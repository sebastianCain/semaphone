#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <string.h>

void printStory() {
  int fd = open("resource.txt", 0, 0666);
  void *buff = malloc(1000);
  read(fd, buff, 1000);
  printf("%s\n", buff);
}

int main(int argc, char *argv[]) {
  char *flag = argv[1];

  if (strcmp(flag, "-c") == 0) {

    //create file
    int fd = open("resource.txt", O_CREAT | O_EXCL, 0666);
    
    //shared mem
    int shmd = shmget(ftok("resource.txt", 1234), 100, IPC_CREAT | 0666); 
    //void *shmaddr = malloc(100);
    //shmat(shmd, shmaddr, 0);

    //semaphores
    int semd  = semget(ftok("resource.txt", 5678), 1, IPC_CREAT | 0666);
    union semun su;
    su.val = 1;
    semctl(semd, 0, SETVAL, su);

  } else if (strcmp(flag, "-r") == 0) {
    
    printStory();
    
    //shared mem
    int shmd = shmget(ftok("resource.txt", 1234), 100, 0666);
    shmctl(shmd, IPC_RMID, NULL); 
    //void *shmaddr = shmat(shmd, 0, 0);
    //shmdt(shmaddr);

    //semaphores
    int semd  = semget(ftok("resource.txt", 5678), 1, IPC_CREAT | 0666);
    semctl(semd, 0, IPC_RMID, NULL);

  } else if (strcmp(flag, "-v") == 0) { 
    printStory();
  }
  return 0;
}
