////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      sendsig.c, mySigHandler.c
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
#include <stdlib.h>
#include <unistd.h>
int count = 0; //global counter variable.

/*programmer defined SIGINT signal handler
 */
void handler_SIGINT(){
        printf("\nTotal number of operations completed successfully: %d\n", count);
        printf("The program will be terminated.\n");
        exit(0);
}

/*programmer defined SIGFPE signal handler
 */
void handler_SIGFPE(){
	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %d\n", count);
        printf("The program will be terminated.\n");
        exit(0);
}

int main(){
	struct sigaction key_int; //struct for SIGINT signal.
        memset(&key_int, 0, sizeof(key_int));
        key_int.sa_handler = handler_SIGINT;
        if(sigaction(SIGINT, &key_int, NULL) !=0){//check binding.
                printf("Error binding SIGINT handler\n");
                exit(1);
        }

	struct sigaction zero_int; //struct for SIGFPE signal.
	memset(&zero_int, 0, sizeof(zero_int));
	zero_int.sa_handler = handler_SIGFPE;
        if(sigaction(SIGFPE, &zero_int, NULL) !=0){//check binding.
                printf("Error binding SIGFPE handler\n");
                exit(1);
        }

	while(1){ //infinite loop.
		printf("Enter first integer: "); //user prompt
		int size_buff = 100;
		char buff[size_buff];
		fgets(buff, size_buff, stdin);//user input
		printf("Enter second integer: ");//usesr prompt
		int size_buff2 = 100;
		char buff2[size_buff2];
		fgets(buff2, size_buff2, stdin);//user input.

		int x = atoi(buff);
		int y = atoi(buff2);
		int q = (int)(x/y);
		int r = x%y;
		printf("%d / %d is %d with a remainder of %d \n", x, y, q, r);
		count++;
	}
	return 0;

}

