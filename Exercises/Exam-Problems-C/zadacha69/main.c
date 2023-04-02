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

void closefd(int fd) {
	int n = errno;
	if(close(fd) == -1) {
		err(2, "close!\n");
	}
	errno = n;
}

int main (const int argc, const char* const argv[]) {

	while(1) {
		int x[2];
		if (pipe(x) == -1) {
			err(1, "pipe!\n");
		}
		pid_t ch = fork();
		if(ch == -1) {
			err(2, "Unable to fork!\n");
		}
		if (ch == 0) {
			close(x[0]);
			dup2(x[1], 1);
			int buffer;
			if (read(0, &buffer, sizeof(int)) != sizeof(int)) {
				err(2, "Read!\n");
			}	

			if (strcmp(buffer, "exit") == 0) {
				exit(0);
			}

			if (execlp(buffer, buffer, (char*)NULL) == -1) {
				err(3, "execlp!\n");
			}
		}
		close(x[0]);
		int status;
		char c;
		while(read(0, &c, 1) != sizeof(1)) {
			if (c == '\n') {
				close(x[1]);
				wait(&status);
				if(WIFEXITED(status) && WEXITSTATUS(status) == -1) {
					exit(0);
				}
				else break;
			}
			else {
				if(write(x[1], &c, 1) != sizeof(1)){
					err(4, "write!\n");
				}
			}
		}
	}
		exit(0);
}
