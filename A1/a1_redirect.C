//  a1_redirect
//
//  Created by Daniele Bercovici on 2018-01-16.
//  Copyright © 2018 Daniele Bercovici. All rights reserved.
//

#include<stdio.h>
#include<unistd.h>
int main()
{
    printf("First :print to stdout\n");
    int file = open("redirect_out.txt", O_WRONLY | O_APPEND);
    dup2(file, 1) ; 
    //freopen("redirect_out.txt", "w", stdout); //of file = open() 
    //dup2(file,2) etc...???
    printf("Second : Print to redirect_out.txt\n");
    //freopen( "CON", "w", stdout );
    
    printf("Third: Print to stdout\n");

    return -1;
}
    