////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c, division.c
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
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

unsigned my_alarm = 3;//variable to store alarm duration.
int count = 0;//variable to store counter value.

/*programmer defined handler for signal SIGALRM
 */
void handler_SIGALRM(){
	time_t time_now; //variable to store time.
	time(&time_now);
	printf("PID: %d CURRUNT TIME: %s", getpid(), ctime(&time_now));
	alarm(my_alarm);//reset the alarm.
	return;
}

/*programmer defined handler for signal SIGUSR1
 */
void handler_SIGUSR1(){
	printf("SIGUSR1 handled and counted!\n");
	//printf("global counter = %d \n", ++count);
	count++;
}

/*programmer defined handler for signal SIGINT
 */
void handler_SIGINT(){
	printf("\nSIGINT handled.\n");
	printf("SIGUSR1 was handled %d times. Exiting now.\n", count);
	exit(0);
}

int main(){
	alarm(my_alarm);//arm the alarm

	struct sigaction act;//struct for SIGALRM signal.
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler_SIGALRM;
	if(sigaction(SIGALRM, &act, NULL) !=0){//check binding
		printf("Error binding SIGALRM handler\n");
		exit(1);
	}

	struct sigaction usr_act;//struct for SIGUSR1 signal
	memset(&usr_act, 0, sizeof(usr_act));
	usr_act.sa_handler = handler_SIGUSR1;
	if(sigaction(SIGUSR1, &usr_act, NULL) !=0){//check binding
		printf("Error binding SIGUSR1 handler\n");
		exit(1);
	}

	struct sigaction key_int;//struct for SIGINT signal.
	memset(&key_int, 0, sizeof(key_int));
	key_int.sa_handler = handler_SIGINT;
	if(sigaction(SIGINT, &key_int, NULL) !=0){//check binding
		printf("Error binding SIGINT handler\n");
		exit(1);
	}

	printf("PID and time print every 3 seconds.\n");
	printf("Type Ctrl-C to end the program.\n");
	while(1){//infinite loop.
	}
	return 0;
}
