#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Patch {
	uint16_t shift;
	uint8_t originalByte;
	uint8_t newByte
};

int main (int argc, char* argv[]) {
	if (argc != 4) {
		errx(1, "Not enough arguements!\n");
	}
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(1,"Opening Failure!\n");
	}	

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1) {
		close(fd1);
		err(1,"Opening Failure!\n");
	}
	
	int fd3 = open(argv[3], O_CREAT | O_RDWR| O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd3 == -1) {
		close(fd1);
		close(fd2);
		err(1,"Opening Failure!\n");
	}

	uint16_t shift;
	uint8_t originalByte;
	uint8_t newByte;

	struct Patch patch;

	ssize_t read_size;

	while((read_size = read(fd1, &originalByte, sizeof(originalByte)))) {
		if (read(fd2, &newByte, sizeof(newByte)) != sizeof(newByte)) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(2,"Reading Failure!\n");
		}
		if (newByte != originalByte) {
			patch.shift = shift;
			patch.originalByte = originalByte;
			patch.newByte = newByte;

			if (write(fd3, &patch , sizeof(patch))!=sizeof(patch)) {
				close(fd1);
				close(fd2);
				close(fd3);
				err(2,"Writting Failure!\n");
			}
		}
		shift++;
	}

	if (read_size == -1) {
		close(fd1);
		close(fd2);
		close(fd3);
		err(2,"Reading Failure!\n");
	}
		
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
