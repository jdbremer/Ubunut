#include <stdio.h>
#include <unistd.h>

int main(){
	int pid;
	
	pid = getpid();
	printf ("child: pid = %d, done.\n", pid);
	
}