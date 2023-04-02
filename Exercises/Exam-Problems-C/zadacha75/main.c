#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>

int main(const int argc, const char* const argv[]) {
	if (argc != 3) {
		errx(1, "Number!\n");
	}

	int p[2];
	if (pipe(p) == -1) {
		err(2, "Pipe failed!\n");
	}

	pid_t pid = fork();
	if(pid == -1) {
		err(3, "Fork failed!\n");
	}

	if (pid == 0) {
		close(p[0]);
		dup2(p[1], 1);
		if(execlp("cat", "cat", argv[1], (char*)NULL) == -1){
			err(4, "execlp!\n");
		}
	}
	close(p[1]);

	int fd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		err(5, "Open failed!\n");
	}

	char c;
	ssize_t rs;

	while ((rs = read(p[0], &c, sizeof(char))) > 0) {
		if (c == 0x7D) {
			if (read(p[0], &c, sizeof(char)) == -1) {
				err(6, "Read failed!\n");
			}
			c ^= 0x22;
			if (write(fd, &c , sizeof(char)) != sizeof(char)) {
				err(7, "Write failed!\n");
			}	
		}
		else if (c == 0x55) {
			continue;
		}	
	}
	if (rs == -1 || rs != sizeof(char)) {
		err(6, "Read failed!\n");
	}

	if (close(p[0]) == -1) {
		err(8, "Close failed!\n");
	}

	if (close(fd) == -1) {
		err(8, "Close failed!\n");
	}
	
	exit(0);
}
