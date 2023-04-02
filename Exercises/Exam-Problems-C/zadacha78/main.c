#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(const int argc, const char* const argv[]) {
	if(argc != 3){
		errx(1, "Number of arguements!\n");
	}
	
	int argv1 = atoi(argv[1]);
	int argv2 = atoi(argv[2]);

	int x[2];
	if(pipe(x) == -1) {
		err(3, "Unable to pipe!\n");
	}

	int y[2];
	if(pipe(y) == -1) {
		err(3, "Unable to pipe!\n");
	}

	pid_t pid = fork();
	if(pid == -1) {
		err(2, "Unable to fork!\n");
	}
	
	if (pid > 0) {
		close(x[0]);
		close(y[1]);
	}
	if(pid == 0) {
		close(x[1]);
		close(y[0]);
	}

	int n = 0;
	write(y[1], &n, sizeof(int));
	for(int i=0; i<argv1; i++) {
		if (pid > 0) {
			read(y[0], &n, sizeof(int));
			if(n == 0) {
				printf("Ding!\n");
				n=1;
				sleep(argv2);
				write(x[1], &n, sizeof(int));
			}
		}
		if(pid == 0) {
			read(x[0], &n, sizeof(int));
			if(n == 1) {
				printf("Dong!\n");
				n=0;
				write(y[1], &n, sizeof(int));
			}
		}	
	}

	exit(0);
}




