#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int closefd(int fd) {
	int n = errno;
	if (close(fd) == -1) {
		err(7, "Unable to close fd!\n");
	}	
	errno = n;	
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "Not enough arguements!\n");
	}

	int fd;
	u_int8_t c;
	u_int8_t counting[256];

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		err(2, "%s", argv[1]);
	} 

	ssize_t read_size;

	while((read_size = read(fd, &c, 1))) {
		if (read_size == -1) {
			closefd(fd);
			err(9,"There was problem in closing the file!\n");
		}
		counting[c]++;
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		closefd(fd);
		err(8, "Lseek problem!\n");
	}
	for (u_int8_t i=0; i<255; i++) {
		for(u_int8_t j = 0; j < counting[i]; i++) {
			if (write(fd, &counting[i], sizeof(i)) != sizeof(i)) {
				close(fd);
				err(2, "Writting failure!\n");
			}
		}
	}
	
	closefd(fd);
	exit(0);
}
