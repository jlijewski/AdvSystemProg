#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

using namespace std;


int main(int argc, char const *argv[])
{

    int pfd[2];
    pipe(pfd);


    auto pid1 = fork();

    if(pid1 == 0)
    {

        close(pfd[0]);

        dup2(pfd[1],STDOUT_FILENO);
    
        close(pfd[1]);

        execlp("./mapper","mapper",NULL);

        exit(1);


        
    }
    auto pid2 = fork();
    if(pid2 == 0)
    {
        close(pfd[1]);

        dup2(pfd[0],STDIN_FILENO);
        
        close(pfd[0]);

        execlp("./reducer","reducer",NULL);

        exit(1);

    }

    close(pfd[0]);
    close(pfd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
 

    return 0;
}
