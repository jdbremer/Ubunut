//***Interface.c file***

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include 
#include <unistd.h>
#define BUFFER_SIZE 2048

void clearBuffer(char * input);
void errcheck(int err);

int main()
{
	int DBreturn = -1;
	int err;
	int Status=0;
	int writeError, readError;
	int toDB[2];
	int toInterface[2];
	char args1[50];
	char args2[50];

	char ReadBuffer[BUFFER_SIZE+1]; //Input
	char WriteBuffer[BUFFER_SIZE+1]; //Cmd

	int DBpid;

	//Create Pipes
	pipe(toDB);
	pipe(toInterface);
	//fork child process
	DBpid = fork();

	if(DBpid == -1){
		printf("There was an error on fork. Error Number: %d\n", errno);
		exit(-1);
	}
	else if (DBpid == 0){
		//in child process
		//Close unused pipe ends
		close(toDB[1]);
		close(toInterface[0]);
		printf("Child ID: %d\n toDB :%d\n toInt :%d\n", getpid(),toDB[0], toInterface[1]);
		//Assign args
		sprintf(args1,"%d", toDB[0]);
		sprintf(args2,"%d", toInterface[1]);
		//Execute DB Program
		err = execl("./DB",args1,args2,NULL);

		if(err == -1){
			printf("It looks like there was an issue with starting DB \n");
			printf("Error Number :%d", errno);
			exit(-1);
		}

		//DB is initialized.
	  
	}else{ // in Parent process
		//Close unused pipe ends
		close(toDB[0]);
		close(toInterface[1]);

		//Pipes are established
		//Expecting message from DB with a success.
		printf("Reading from DB\n\n"); //debugging
		readError = read(toInterface[0],ReadBuffer, 99);
		printf("Response: %s", ReadBuffer);
		clearBuffer(ReadBuffer);
		errcheck(readError);
		  
		//Begin menu loop
		int Done = 0;
		while(Done == 0 ){

			printf("Please supply a command( 'account, id' | list | date,mm/dd/yy | exit)\n:");
			  
			//recieve input from user, send to DB.
			clearBuffer(WriteBuffer);
			scanf("%s", WriteBuffer);
			writeError = write(toDB[1], WriteBuffer, BUFFER_SIZE);
			errcheck(writeError);
			  
			//Read Response from DB
			readError = read(toInterface[0], ReadBuffer, BUFFER_SIZE);
			errcheck(readError);
			printf("Response: %s\n", ReadBuffer);
			  
			//Check for exit return by DB
			if(atoi(ReadBuffer) == 1){
				Done = 1;
			}
			  
			clearBuffer(ReadBuffer);
		}
		//Wait for child process to finish;
		err = waitpid(-1, &Status ,0);
		errcheck(err);
		  
		printf("Goodbye!\n");

	}//End of Parent Instructions

	return 0; //Exit
}

void clearBuffer(char *input){
	int i = 0;
	for(i; i < BUFFER_SIZE; i++){
		*(input+i) = '\0';
	}
}

void errcheck(int err){

	if(err == -1){
		printf("It looks like there was an issue...");
		printf("Error Number :%d\n", errno);
		exit(-1);
	}
}

