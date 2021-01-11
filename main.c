
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int main (int argc, char **argv, char **env)
{
    char s = 'c';// = 'c';
    int fd[2];
    int n;
    //pipe(fd);
    int i = fork();
    int     d;
    if (i > 0)
    {
        for(int i = 0 ;i < 100;i++)
            printf("|%d|", i);
    }
    else if (i == 0)
    {
        printf("baaa3\n");
        // fflush(stdin);
        // fflush(stdout);
        // fflush(stderr);
        int by = read(0, &s, 1);
        printf("%d\n", by);;
        write(1, &s, 1);
    }
    // wait(NULL);

}
