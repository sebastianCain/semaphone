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
    int semd = semget(ftok("resource.txt", 5678), 1, 0);
    
    struct sembuf b;
    b.sem_op = -1;
    b.sem_num = 0;
    b.sem_flg = SEM_UNDO;
    
    semop(semd, &b, 1);
    
    int shmd = shmget(ftok("resource.txt", 1234), 100, 0);

    // faulty code after this comment
    
    int * shmaddr = shmat(shmd, 0, 0);  //size??
    int fd = open("resource.txt", O_RDWR, 0644);
    char *line;
    lseek(fd, -(*shmaddr), SEEK_END);
    read(fd, line, *shmaddr);
    printf("Next line:");
    fgets("resource.txt", 100, stdin);
    
    printf("%d\n", *shmaddr);
}
