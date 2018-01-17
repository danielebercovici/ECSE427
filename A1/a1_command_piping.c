#include<stdio.h>
#include<unistd.h>
#include <fcntl.h> 

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

        //child : executed ls using execvp and redirect into input of pipe
        //close(stdout);
        close(mypipe[0]); //close reading
        dup2(mypipe[1], 1);
        char *argm[] = {"ls", "-la", 0}; 
        execvp(argm[0], argm);
        //execvp("ls", NULL);
    }
    else{
        //parent : print output from ls here
        close(mypipe[1]); //close writing
        dup2(1, mypipe[0]);
        
    }
    return 0;
}