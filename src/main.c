
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *myRoutine(void *me) {
	while(1) {
		printf("test");
	    sleep(1);
	}
}

int main(int argc, char *argv[], char *envp[]) {


	pthread_t tid;
	pthread_create(&tid, NULL, myRoutine, NULL);
	pthread_join(tid, NULL);

	return 0;
}
