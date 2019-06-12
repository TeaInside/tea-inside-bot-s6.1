
#include <stdio.h>
#include <teabot/teabot.h>
#include <teabot/config/config_parser.h>

int main(int argc, char *argv[], char *envp[]) {

	if (init_config(argc, argv, envp)) {
		run_daemon();
	}
	
	return 0;
}


#ifdef QQQQQ
int main(){
    int fd[4];
    enum {
        thread1_read  = 0, // read end for parent
        thread2_write = 1, // write end for child 
        thread2_read  = 2, // read end for child 
        thread1_write = 3  // write end for parent
    };
    if (pipe(&fd[thread1_read]) == -1 || (pipe(&fd[thread2_read]) == -1)){
        perror("In pipe");
        return 1;
    }
    switch(fork()) {
        case -1:
            perror("In fork()");
            return 1;
        case 0:{ //it's a child
            char buf[256];
            memset(buf, 0, sizeof(buf));
            if (wait_n_read(fd[thread2_read], buf, sizeof(buf)-1) == -1)
                return 1;
            printf("Child: %s\n", buf);
            const char helloback[] = "Hello back\n";
            sleep(3);
            write(fd[thread2_write], helloback, 12);
            return 0;
        }
        default: { //a parent
            const char hello[] = "Hello\n";
            write(fd[thread1_write], hello, sizeof(hello));
            char buf[256];
            memset(buf, 0, sizeof(buf));
            if (wait_n_read(fd[thread1_read], buf, sizeof(buf) - 1) == -1)
                return 1;
            printf("Parent: %s\n", buf);
            fflush(stderr);
        }
    }
}
#endif