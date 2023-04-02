#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(const int argc, const char* const argv[]) {
	if (argc != 2) {
		errx(1, "lkfzng");
	}
	
	int p1[2];
	if(pipe(p1) == -1) {
		err(2, "Pipe!\n");
	}	
	
	pid_t pid = fork();
	if (pid == -1) {
		err(3, "Unable to fork!\n");
	}

	if (pid == 0) {
		close(p1[0]);
		dup2(p1[1], 1);
		if (execlp("find", "find", argv[1], "-type", "f", "-printf", "%C@ %p\n", (char*)NULL) == -1) {
			err(4, "execlp problem!\n");
		}
	}

	int p2[2];
	if(pipe(p2) == -1) {
		err(2, "Pipe!\n");
	}
	pid_t pid2 = fork();
	if (pid2 == -1) {
		err(3, "Unable to fork!\n");
	}

	if (pid2 == 0) {
		close(p2[0]);
		close(p1[1]);
		dup2(p1[0], 0);
		dup2(p2[1], 1);
		if (execlp("sort", "sort", "-nr", "-k1", (char*)NULL) == -1) {
			err(4, "execlp problem!\n");
		}
	}

	close(p1[1]);
	close(p1[0]);
	close(p2[1]);
	dup2(p2[0], 0);
	if (execlp("head", "head", "-n", "1", (char*)NULL) == -1) {
		err(4, "execlp problem!\n");
	}	

	exit(0);
}
