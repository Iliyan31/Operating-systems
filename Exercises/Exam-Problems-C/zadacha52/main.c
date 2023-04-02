#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>

struct IDX {
	uint16_t offset;
	uint8_t length;
	uint8_t	useless;
};

int main (int argc, char* argv[]) {
	if (argc != 5) {
		errx(1, "There must be 4 parameters!\n");
	}
	
	struct stat f1_dat;
	if (stat(argv[1], &f1_dat) == -1) {
		err(2,"Stat Failed!\n");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err(3, "Open Failed!\n");
	}

	struct stat f1_idx;
	if(stat(argv[2], &f1_idx) == -1) {
		err(2,"Stat Failed!\n");
	}

	int fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1) {
		close (fd1);
		err(3, "Open Failed!\n");
	}
	
	int fd3 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd3 == -1) {
		close(fd1);
		close(fd2);
		err(3, "Open Failed!\n");
	}

	int fd4 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd4 == -1) {
		close(fd1);
		close(fd2);
		close(fd3);
		err(3, "Open Failed!\n");
	}
	
	struct IDX idx;
	ssize_t readsize;
	while ((readsize = read(fd2, &idx, sizeof(idx)))) {
		if(readsize == -1 || readsize != sizeof(struct IDX)) {
			err(2, "Reading Failure!\n");
		}
		if (lseek(fd1, idx.offset, SEEK_SET) == -1) {
			err(10, "kdzm");
		}		
		uint8_t* arr = malloc(idx.length);
		if(arr == NULL) {
			err(5, "No dynamic memory allocated!\n");
		}		
		readsize = read(fd1, arr, sizeof(arr));
		if (readsize == -1) {
			free(arr);
			err(2, "Reading Failure!\n");
		}

		if (arr[0] >= 0x41 && arr[0] <= 0x5A) {
			if (write(fd3, arr, sizeof(arr))!= sizeof(arr)) {
				free(arr);
				err(2, "Writting Failure!\n");
			}
			if (write (fd4, &idx, sizeof(idx)) != sizeof(idx)) {
				free(arr);
				err(2, "Writting Failure!\n");
			}
		}
		free(arr);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	exit(0);
}
