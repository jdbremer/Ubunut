#include <iostream>
#include <string>
#include <mutex>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
const int TOTALMEM = 100;
const int MAXDEGREEMP = 5;
const int MAXPROCESSES = 3;
pthread_mutex_t Out = PTHREAD_MUTEX_INITIALIZER;
void println (char *s) {
pthread_mutex_lock (&Out);
cout << s << endl;
pthread_mutex_unlock (&Out);
}
struct Memory {
int id;
int size;
};
class Scheduler {
private:
int id;
int totMem;
int freeMem;
Memory memory[MAXDEGREEMP+1];
int next;
public:
Scheduler ();
Scheduler (int);
void displayProcesses ();
void getMemory (int id, int need);
void returnMemory (int id, int need);
};
Scheduler::Scheduler () {
totMem = 0;
freeMem = 0;
next = 0;
}
Scheduler::Scheduler (int totMem) {
totMem= totMem;
freeMem = totMem;
for (int i = 0; i < MAXDEGREEMP+1; i++) {
memory[i].id = 0; memory[i].size = 0;
}
next = 0;
}
void Scheduler::displayProcesses () {
if ( next == 0) return;
string pl = "";
for (int i = 0; i < next; i++)
pl = pl + "Process " + to_string(memory[i].id) +
": memory size = " + to_string(memory[i].size) + " ";
pthread_mutex_lock (&Out);
cout << pl << endl;
pthread_mutex_unlock (&Out);
}
void Scheduler::getMemory (int id, int need) {
int err;
char gmstr[100];
freeMem = freeMem - need;
memory[next].id = id;
memory[next].size = need;
sprintf (gmstr, "getMemory: assigned id %d memory size %d", id, need);
println (gmstr);
next++;
displayProcesses ();
return;
}
void Scheduler::returnMemory (int id, int need) {
int err;
char rmstr[100];
bool found = false;
for (int i = 0; i < MAXDEGREEMP; i++) {
if (id == memory[i].id) {
found = true;
freeMem = freeMem + memory[i].size;
for (int j = i + 1; j < next; j++) {
memory[j-1].id = memory[j].id;
memory[j-1].size = memory[j].size;
}
next--;
memory[next].id = 0;
memory[next].size = 0;
break;
}
}
if (!found) {
sprintf (rmstr, "Error in program: %d -- %d", id, need);
println (rmstr);
}
sprintf (rmstr, "returnMemory: returned id %d memory size %d", id, need);
println (rmstr);
displayProcesses ();
return;
}
class Process {
private:
int id;
unsigned int sid; /* seed for rand */
Scheduler* scheduler;
int neededMemory;
public:
Process (int, Scheduler*);
void operator () ();
void run ();
};
Process::Process (int identifier, Scheduler* schd) {
id = identifier;
sid = id;
scheduler = schd;
neededMemory = 1 + rand_r(&sid) % (TOTALMEM / 2);
}
void Process::run () {
char rstr[100];
scheduler->getMemory (id, neededMemory);
sleep (1);
scheduler->returnMemory (id, neededMemory);
sprintf (rstr, "process %d finished", id);
println ( rstr );
}
void *callRun (void* process) {
((Process *)process) -> run();
}
int main (int argc, const char * argv[]) {
int err;
pthread_t tidA, tidB;
Scheduler* schd = new Scheduler (TOTALMEM);
Process *processA = new Process (1, schd);
Process *processB = new Process (2, schd);
err = pthread_create (&tidA, NULL, callRun, processA);
if (err) { cout << "create of Process thread failed." << endl; exit(1);};
err = pthread_create (&tidB, NULL, callRun, processB);
if (err) { cout << "create of Process thread failed." << endl; exit(2);};
pthread_mutex_lock (&Out);
cout << "All processes are active." << endl;
pthread_mutex_unlock (&Out);
pthread_join(tidA, NULL);
pthread_join(tidB, NULL);
cout << "All processes have completed." << endl;
return 0;
}
