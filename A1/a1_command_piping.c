#include <unistd.h>
#include <string.h>

int main()
{
   int mypipe[2];
   pipe (mypipe);

   if (fork() == 0)
   {
      close(mypipe[0]);
      dup2(mypipe[1], 1);

      char *argm[] = {"ls", "-la", 0};
      execvp(argm[0], argm); //execute command ls
   }
   else
   {
      char buffer[1024] = {0};

      close(mypipe[1]);

      while (read(mypipe[0], buffer, sizeof(buffer)) != 0)
      {
         write(1, buffer, strlen(buffer));
         memset (buffer, 0, sizeof(buffer));
      }
   }
   return 0;
}