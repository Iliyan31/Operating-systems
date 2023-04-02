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
		errx(1, "kajfhjk!\n");
	}	

	if(mkfifo("74-fifo", 0644) == -1) {
		err(2, "fifo failed!\n");
	}

	int fd = open("74-fifo", O_WRONLY);
	if (fd == -1) {
		err(3, "Open failed\n");
	}

	dup2(fd, 1);
	if (execlp("cat", "cat", argv[1], (char*)NULL) == -1) {
		err(4, "Execlp failed!\n");
	}

	exit(0);	
}
