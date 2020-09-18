#ifndef PROJ2HDR_H
#define PROJ2HDR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define displayMenu() printf("Input command (account,id | list | date,mm/dd/yyyy | exit\n")

#define DATELEN 11
#define BUFFERLEN 4096
#define CBSIZE 100
#define PARENT_READ db2[0]
#define PARENT_WRITE db1[1]
#define CHILD_WRITE db2[1]
#define CHILD_READ  db1[0]
#define ASKDATE printf("Enter a date (MM/DD/YYYY): \n")

typedef struct record {
	int id, checknumber;
	char date[DATELEN];
	float money;
}checkbookentry;

void swapcb(checkbookentry cb[], int cb1, int cb2){
	int temp1 = cb[cb1].id, temp2 = cb[cb1].checknumber;
	char temp3[DATELEN];
	float temp4 = cb[cb1].money;
	strcpy(temp3, cb[cb1].date);

	cb[cb1].id = cb[cb2].id;
	cb[cb1].checknumber = cb[cb2].checknumber;
	strcpy(cb[cb1].date, cb[cb2].date);
	cb[cb1].money = cb[cb2].money;

	cb[cb2].id = temp1;
	cb[cb2].checknumber = temp2;
	strcpy(cb[cb2].date, temp3);
	cb[cb2].money = temp4;
}
//
void printcbentry(checkbookentry cb, char response[]){
	char st[16];
	strcat(response, "\nid = ");
	sprintf(st,"%d",cb.id);
	strcat(response, st);
	strcat(response, ", check number: ");
	sprintf(st,"%d",cb.checknumber);
	strcat(response, st);
	strcat(response, ", date: ");
	strcat(response, cb.date);
	strcat(response, ", amount: $");
	sprintf(st,"%.2f",cb.money);
	strcat(response, st);
}
void printcbarr(checkbookentry cb[], int sizecb, char response[]){
	for(int i = 0; i < sizecb; i++){
		printcbentry(cb[i],response);
	}
}
int comparedate(char ls[],char rs[]){ //returns 1 if ls < rs returns 0 if ls > rs returns 2 if equal
	if(ls[0] < rs[0])
		return 1;
	else if(ls[0] > rs[0])
		return 0;
	if(ls[1] < rs[1])
		return 1;
	else if(ls[1] > rs[1])
		return 0;
	if(ls[3] < rs[3])
		return 1;
	else if(ls[3] > rs[3])
		return 0;
	if(ls[4] < rs[4])
		return 1;
	else if(ls[4] > rs[4])
		return 0;
	if(ls[6] < rs[6])
		return 1;
	else if(ls[6] > rs[6])
		return 0;
	if(ls[7] < rs[7])
		return 1;
	else if(ls[7] > rs[7])
		return 0;
	if(ls[8] < rs[8])
		return 1;
	else if(ls[8] > rs[8])
		return 0;
	if(ls[9] < rs[9])
		return 1;
	else if(ls[9] > rs[9])
		return 0;
	return 2;
}
void sortbyid(checkbookentry cb[], int sizecb,char response[]){
	int i, j, k, firstdup, lastdup;
	for(j = 0; j < (sizecb - 1); j++){
		for(i = j; i < (sizecb - 1); i++){
			if(cb[i+1].id < cb[j].id){
				swapcb( cb, i+1, j);
			}
		}
	}
	for(i = 0; i < (sizecb - 1); i++){
		for(j = i; j < (sizecb - 1); j++){
			if(cb[j+1].id == cb[i].id){
				if(comparedate(cb[j+1].date, cb[i].date) == 1){
					swapcb( cb, i, j+1);
				}
			}
			else{
				j = sizecb - 1;
			}
		}
	}
	printcbarr(cb, sizecb, response);
}
void sortbydate(checkbookentry cb[], int sizecb, char response[]){
	int i, j, k, firstdup, lastdup;
	for(j = 0; j < (sizecb - 1); j++){
		for(i = j; i < (sizecb - 1); i++){
			if(comparedate(cb[i+1].date, cb[j].date) == 1){
				swapcb( cb, i+1, j);
			}
		}
	}
	for(i = 0; i < (sizecb - 1); i++){
		for(j = i; j < (sizecb - 1); j++){
			if(comparedate(cb[j+1].date, cb[i].date) == 2){
				if(cb[j+1].id < cb[i].id){
					swapcb( cb, i, j+1);
				}
			}
			else{
				j = sizecb - 1;
			}
		}
	}
	printcbarr(cb, sizecb, response);
}
float totalmon(checkbookentry cb[], int sizecb){
	float total = 0.00;
	for(int i = 0; i < sizecb; i++){
		total += cb[i].money;
	}
	return total;
}

void cbdatetotal(checkbookentry cb[], int sizecb, char date[], char response[]){
	float total = 0;
	char st[16];
	for(int i = 0; i < sizecb; i++){
		if(comparedate(cb[i].date, date) == 2){
			total += cb[i].money;
		}
	}
	
	strcpy(response, "\nresponse: total on date ");
	strcat(response, date);
	strcat(response, ": $");
	sprintf(st,"%.2f",total);
	strcat(response, st);
}
int isaccount(char cmd[]){
	if(cmd[0] == 'a' && cmd[1] == 'c' && cmd[2] == 'c' && cmd[3] == 'o' && cmd[4] == 'u' && cmd[5] == 'n' && cmd[6] == 't'){
		return 1;
	}
	return 0;
}
int islist(char cmd[]){
	if(cmd[0] == 'l' && cmd[1] == 'i' && cmd[2] == 's' && cmd[3] == 't'){
		return 1;
	}
	return 0;
}
int isdate(char cmd[]){
	if(cmd[0] == 'd' && cmd[1] == 'a' && cmd[2] == 't' && cmd[3] == 'e'){
		return 1;
	}
	return 0;
}
int isexit(char cmd[]){
	if(cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't'){
		return 1;
	}
	return 0;
}
void cbaccounttotal(checkbookentry cb[], int data, int sizecb, char response[]){
	float total = 0;
	char st[16];
	for(int i = 0; i < sizecb; i++){
		if(data == cb[i].id){
			total += cb[i].money;
		}
	}
	sprintf(st,"%d",data);
	strcpy(response, "response: total for account ");
	strcat(response, st);
	strcat(response, ": $");
	sprintf(st,"%.2f",total);
	strcat(response, st);

}
void docommand(checkbookentry cb[], char command[], int sizebuff, int usedcb, char response[]){
	int i, data; //data is used for account number and checknumber
	memset(response,0 ,4096);
	char date[DATELEN], exitt[] = "exit";
	if(isaccount(command) == 1){
		for(i = 8; i < strlen(command); i++){
			date[i - 8] = command[i];
		}
		data = atoi(date);
		memset(command,0,BUFFERLEN);
		cbaccounttotal(cb, data, usedcb, response);
		return;
	}
	else if(islist(command) == 1){
		//printf("list option entered\n");
		memset(command,0,BUFFERLEN);
		printcbarr(cb, usedcb, response);
		return;
	}
	else if(isdate(command) == 1){
		for(i = 5; i < (5 + DATELEN); i++){
			date[i - 5] = command[i];
		}
		memset(command,0,BUFFERLEN);
		cbdatetotal(cb,usedcb,date, response);
	}
	else if(isexit(command) == 1){
		memset(command,0,BUFFERLEN);
		strcpy(command, exitt);
	}
	else{
		strcat(response, "error: invalid command given.");
	}
	strcat(response, "\n");
	return;	
}

#endif