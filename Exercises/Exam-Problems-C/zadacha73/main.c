#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdbool.h>
#include <time.h>

int main(const int argc, char* const argv[]) {
	if(argc < 3) {
		errx(1, "Number of arguements!\n");
	}
	
	int fd = open("run.log", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		err(2, "Open failed!\n");
	}
	
	int num = argv[1][0] - '0';
	bool flag= false;
	while(true) {
		int p[2];
		if (pipe(p) == -1) {
			err(3, "Pipe failed!\n");
		}	

		pid_t pid = fork();
		if (pid == -1) {
			err(4, "Fork failed!\n");
		}

		if (pid == 0) {
			int start = time(NULL);
			close(p[0]);
			if (write(p[1], &start, sizeof(int)) != sizeof(int)) {
				err(5, "Write failed!\n");
			}
			if(execvp(argv[2], argv + 2) == -1) {
				err(6, "Execvp problem!\n");
			}			
		}

		close(p[1]);
		int status;
		wait(&status);
		int finish = time(NULL);
		int endStatus = 0;
		if (WIFEXITED(status) && WEXITSTATUS(status) !=0) {
			endStatus = WEXITSTATUS(status);
		}
		
		int start;
		if(read(p[0], &start, sizeof(int)) != sizeof(int)) {
			err(6, "Reading failed!\n");
		}

		if(write(fd, &start, sizeof(int)) != sizeof(int)) {
			err(5, "Write failed!\n");
		}
		if(write(fd, " ", 1) != 1) {
			err(5, "Write failed!\n");
		}
		if(write(fd, &finish, sizeof(int)) != sizeof(int)) {
			err(5, "Write failed!\n");
		}
		if(write(fd, " ", 1) != 1) {
			err(5, "Write failed!\n");
		}
		if(write(fd, &endStatus, sizeof(int)) != sizeof(int)) {
			err(5, "Write failed!\n");
		}

		if(endStatus !=0 && finish-start < num) {
			if (flag) {
				exit(0);
			}
			else flag = true;
		}
		else flag = 0;

		close(p[0]);
	}

	exit(0);
}
