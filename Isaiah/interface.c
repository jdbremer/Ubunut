#include "proj2hdr.h"

int main() {
	int db1[2], db2[2];
	int id;
	int err;
	char rbuffer[BUFFERLEN], wbuffer[BUFFERLEN], exitt[] = "exit";
	checkbookentry cb[CBSIZE];
	char arg1[100], arg2[100], arg3[100], arg4[100];

   id = fork();
	err = pipe(db1);
   
   if (err == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   err = pipe(db2);
   
   if (err == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }
	if (id == 0) {
		close(PARENT_WRITE);
		close(PARENT_READ);

      //printf("test output\n");
		sprintf(arg1, "%d", CHILD_READ);
		sprintf(arg2, "%d", CHILD_WRITE);

		err = execl ("./db", arg1, arg2, (char*)NULL);
		if( err == -1 ) {
            printf ("child:  execl failed, errno = %d\n", errno);
            exit (2);
		}


	}
	else if(id > 0){
		close(CHILD_READ);
      close(CHILD_WRITE);

      printf("!!!\n");

      err = read(PARENT_READ, rbuffer ,BUFFERLEN);
      if( err == -1 ) {
            printf ("parent:  read failed, errno = %d\n", errno);
            exit (2);
		}

      printf("\nrbuffer is:\n%s", rbuffer);
      memset(rbuffer, 0 ,sizeof(rbuffer));

      printf("!!!!!\n");

		
      while(strcmp(exitt,wbuffer) != 0){
         displayMenu();
         

         fgets(wbuffer, BUFFERLEN, stdin);
         err = write(PARENT_WRITE, wbuffer, sizeof(wbuffer));
         printf("\nwbuffer is %s\n", wbuffer); 
         memset(wbuffer, 0 ,sizeof(wbuffer));
         
         if( err == -1 ) {
            printf ("child:  write failed, errno = %d\n", errno);
            exit (2);
		   }

         err = read(PARENT_READ, rbuffer, sizeof(rbuffer));
         if( err == -1 ) {
            printf ("child:  read failed, errno = %d\n", errno);
            exit (2);
		   }
         printf("\nrbuffer is \n %s\n", rbuffer);
      }
	}
	else{
		printf("error, fork failed!");
		return 1;
	}
   //close(CHILD_READ);
   //close(CHILD_WRITE);
   //close(PARENT_READ);
   //close(PARENT_WRITE);
   
	return 0;
}
