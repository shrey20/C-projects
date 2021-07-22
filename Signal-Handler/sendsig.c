////////////////////////////////////////////////////////////////////////////////
// Main File:        sendsig.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c, division.c
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Shreyans Sakhlecha
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
int main(int argc, char *argv[]){
        if ( argc != 3){ //check the number of command line arguments.
		printf("Usage: sendsig <signal type> <pid>\n");
		exit(1);
	}
	pid_t my_id; //variable to store pid
	my_id = atoi(argv[2]);
	if(strcmp(argv[1], "-i") == 0){ //check if signal is SIGINT
		kill(my_id, SIGINT);
	}
	else if (strcmp(argv[1], "-u") == 0){ //check if signal is SIGUSR1
		kill(my_id, SIGUSR1);
	}
	else{
		printf("Usage: sendsig <signal type> <pid>\n");
                exit(1);
	}

	return 0;
}
