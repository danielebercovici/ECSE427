//  a1_redirect
//
//  Created by Daniele Bercovici on 2018-01-16.
//  Copyright © 2018 Daniele Bercovici. All rights reserved.
//

#include<stdio.h>
#include<unistd.h>
#include <fcntl.h> 

int main()
{
    printf("First :print to stdout\n");
    int file = open("redirect_out.txt", O_WRONLY | O_APPEND);
    int out=dup(1); // save stdout
    dup2(file, 1) ; 
    printf("Second : Print to redirect_out.txt\n");
    dup2(out, 1); // redirect output back to stdout
	close(file);
	close(out);
    printf("Third: Print to stdout\n");

    return -1;
}
    