#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(const int argc, const char* const argv[]) {
	if(argc != 2) {
		errx(3, "Number of arguements!\n");
	}

	int p[2];
	if(pipe(p) == -1) {
		err(1, "Pipe failed!\n");
	}

	pid_t pid = fork();
	if (pid == -1) {
		err(2, "Pid failed!\n");
	}

	if (pid == 0) {
		close(p[0]);
		dup2(p[1], 1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
			err(4, "Execlp problem!\n");
		}
	}
	
	close(p[1]);
	dup2(p[0], 0);

	if(execlp("sort", "sort", (char*)NULL) == -1) {
		err(4, "Execlp problem!\n");
	}	

	exit(0);
}
