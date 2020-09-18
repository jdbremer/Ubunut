#include "proj2hdr.h"

int main(int arga, char **argb){
	int err, i=0, n, data; //n is the actual size of the chackbook that is used and data is used to store the int value for account number and check number
	char buffer[BUFFERLEN], date[DATELEN], command[BUFFERLEN], response[BUFFERLEN], exitt[] = "exit";
	FILE *db;
	checkbookentry cb[CBSIZE];
	float total;
	int db1[2], db2[2];

	printf("!!!!!!!!!\n");

	sscanf(argb[0],"%d", &CHILD_READ);
	sscanf(argb[1],"%d", &CHILD_WRITE);

	db = fopen("accountData.txt", "r");
	printf("!!!!!!!\n");
	for(i; i<CBSIZE; i++){
		n = fscanf(db, "%d %d %s %f", &cb[i].id, &cb[i].checknumber, cb[i].date, &cb[i].money);
		if(feof(db)){
			n = i + 1;
			break;
		}
	}
	strcpy(date, "list");
	docommand(cb, date, BUFFERLEN, n, response);
	memset(date,0,DATELEN);
	strcat(response, "\nend-of-file detected on file in\n");
	printf("response:%s\n", response);
	write(CHILD_WRITE, response, BUFFERLEN);
	while(strcmp(exitt,response) != 0){
		err = read(CHILD_READ, buffer, BUFFERLEN);
		docommand(cb, command, BUFFERLEN, n, response); //return a string
		err = write(CHILD_WRITE, response, BUFFERLEN);
	}
}