#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <string.h>

int main(void) {
	int x[2];
	if (pipe(x) == -1) {
		err(1, "Pipe failed!\n");
	}

	pid_t pid = fork();
	if(pid == -1) {
		err(2, "Unable to fork!\n");
	}		
	if (pid == 0) {
		close(x[0]);
		dup2(x[1], 1);
		if (execlp("awk", "awk", "-F", ":", "{print $NF}", "/etc/passwd", (char*)NULL) == -1) {
			err(3, "Unable to execute cat!\n");
		}
	}
	close(x[1]);

	int p[2];
	if(pipe(p) == -1) {
		err(4, "Pipe p problem!\n");
	}

	pid_t pid2 = fork();
	if(pid2 == -1) {
		err(5, "Pid2 problem!\n");
	}

	if(pid2 == 0) {
		close(p[0]);
		dup2(x[0], 0);
		dup2(p[1], 1);
		if(execlp("sort", "sort", (char*)NULL) == -1) {
			err(6, "Execlp sort problem!\n");
		}	
	}

	
	close(x[0]);
	close(p[1]);
	dup2(p[0],0);
	if(execlp("uniq", "uniq", "-c", (char*)NULL) == -1) {
		err(6, "Execlp uniq problem!\n");
	}

	


	/*int p[2];
	if (pipe(p) == -1) {
		err(1, "Pipe failed!\n");
	}

	pid_t pid2= fork();
	if(pid2== -1) {
		err(2, "Unable to fork!\n");
	}		
	if (pid2== 0) {
		close(x[1]);
		close(p[0]);

		dup2(x[1], 1);
		if (execlp("cat", "cat", "/etc/passwd", (char*)NULL) == -1) {
			err(3, "Unable to execute cat!\n");
		}
	}*/

	exit(0);
}
