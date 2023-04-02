/*#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void closefd(int fd) {
	int n = errno;
	if (close(fd) == -1) {
		err(2, "Closefd problem!\n");
	}
	errno = n;
}

struct FILE {
	uint32_t x;
	uint32_t y;
};

int main (const int argc, const char* const argv[]) {
	if (argc != 4) {
		err(1, "Invalid number of parameters passed\n");
	}
	
	struct stat st1;
	if (stat(argv[1], &st1) == -1) {
		err(3, "Unable to stat!\n");
	}

	if(st1.st_size % 2 != 0) {
		err(5, ".......");
	}	

	struct stat st2;
	if (stat(argv[2], &st2) == -1) {
		err(3, "Unable to stat!\n");
	}

	if(st2.st_size % 2 != 0) {
		err(5, ".......");
	}	

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(4, "Unalble to open!\n");
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1) {
		err(4, "Unalble to open!\n");
	}
	
	int fd3 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd3 == -1) {
		err(4, "Unalble to open!\n");
	}
	
	struct FILE file;
	ssize_t rs;
	while((rs == read(fd1, &file, sizeof(struct FILE)))) {
		if (rs == -1 || rs != sizeof(struct FILE)) {
			err(6, "Read fail!\n");	
		}
		if (lseek(fd2, file.x, SEEK_SET) == -1) {
			err(7, "Lseek problem!\n");
		}
		
		uint32_t buff;
		ssize_t rs2;
		for(uint32_t i=0; i < file.y; i++) {
			rs2 = read(fd2, &buff, sizeof(uint32_t));
			if (rs2 == -1 || rs2 != sizeof(uint32_t)) {
				err(6, "Read fail!\n");	
			}
			if (write(fd3, &buff, sizeof(uint32_t)) != sizeof(uint32_t)) {
				err(7, "Write failed!\n");
			}
		}
	}
	
	closefd(fd1);
	closefd(fd2);
	closefd(fd3);
	exit(0);
}*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <uinstd.h>
#include <fcntl.h>
#include <err.h>

int main(const int argc, const char* const argv[]) {
	

	exit(0);
}
