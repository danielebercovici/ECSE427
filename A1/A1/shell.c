#define _XOPEN_SOURCE 700 // getline
#define _BSD_SOURCE // strsep
#define MAX_JOBS 1000
#define MAX_ARGS 10

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


/*Command Prompt*/
int getcmd(char *prompt, char *args[], int *background, int *redir)
{
	int i = 0, k = 0;
	char *token, *loc;
	char *line = NULL;
	size_t linecap = 0;
	printf("%s", prompt);
	ssize_t length = getline(&line, &linecap, stdin);
	if (length <= 0) {
		free(line);
		exit(EXIT_FAILURE);
	}
	// Check if background is specified..
	if ((loc = strchr(line, '&')) != NULL) {
		*background = 1;
		*loc = ' ';
	} else
		*background = 0;
	// Check if output redirection is specified..
	if ((loc = strchr(line, '>')) != NULL) {
		*redir = 1;
		*loc = ' ';
	} else
		*redir = 0;

	
	free(line);
	args[i] = NULL;
	return i + k;
}
void executeCommand(int cnt, char *args[], int bg, int redir) {
    if (strcmp(args[0], "cd") == 0) { // Change directory
		if (cnt == 1)
			printf("No directory specified.\n");
		else
			chdir(args[1]);
	} else if (strcmp(args[0], "pwd") == 0) { // Present working directory
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("%s", cwd);
	} else if (strcmp(args[0], "exit") == 0) { // Exit
		//freeMemory(args);
		exit(EXIT_SUCCESS);
	}
}
int main(void)
{
    char *args[MAX_ARGS];
	int bg, redir;
	while (1) {
		bg = 0;
		int cnt = getcmd("\n>> ", args, &bg, &redir);
		if (cnt > 0) {
			executeCommand(cnt, args, bg, redir);
		}
		//freeCommandArguments(args, args2);
	}
}

