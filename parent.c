#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>


int main(){
	int err;
	
	err = fork();
	if (err == -1) {
		printf("parent: fork failed, errno = %d\n", errno);
		exit(1);
	}
	else if (err == 0) {
		err = execl ("./child", "child", (char *)NULL);
		if (err == -1) {
			printf("parent: fork failed, errno = %d\n", errno);
			exit(2);
		}
		
	}
	printf ("parent: child created with pid = %d\n", err);
}