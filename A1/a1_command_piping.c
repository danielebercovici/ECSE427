#include<stdio.h>
#include<unistd.h>
int main(){
    int mypipe[2];
     // Create the pipe. 
     if (pipe (mypipe))
    {
      fprintf (stderr, "Pipe failed.\n");
      return -1;
    }

    if (fork()==0)
    {
        //child : executed ls using execvp and redirct into input of pipe
        close(stdout);
        close(mypipe[0]);//close reading
        dup2(mypipe[1], stdout);
        execvp("ls", NULL);
    }
    else{
        //parent : print output from ls here
        close(mypipe[1]); //close writing
        dup2(stdout, mypipe[0]);
        printf(mypipe[0]);
    }
    return 0;
}