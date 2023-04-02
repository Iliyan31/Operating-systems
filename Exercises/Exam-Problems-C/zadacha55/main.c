#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Patch {
	uint16_t offset;
	uint8_t original;
	uint8_t modified;
};

int main(int argc, char* argv[]) {
	if(argc != 4) {
		errx(1,"There must be four arguements!\n");
	}	
	
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) {
		err (2,"%s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
   	if (fd2 == -1) {
		close(fd1);
		err(2, "%s", argv[2]);	
	}

	int fd3 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd3 == -1) {
		close(fd1);
		close(fd2);
		err(3, "%s", argv[3]);
	}	
		
	uint8_t buffer;
	ssize_t read_size;

	while((read_size = read(fd2, &buffer, sizeof(buffer)))) {
		if (read_size == -1) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(4, "Reading Failure!\n"); 
		}

		if (write(fd3, &buffer, sizeof(buffer))!= sizeof(buffer)) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(5,"Writting Failure!\n"); 
		}
	}

	lseek(fd2, 0, SEEK_SET);
	lseek(fd3, 0, SEEK_SET);

	struct Patch patch;
	uint8_t review;
	
	while((read_size == read(fd1, &patch, sizeof(patch)))) {
		if (read_size == -1) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(6,"Reading Failed!\n");
		}

		lseek (fd1, patch.offset, SEEK_SET);
		read_size = read(fd1, &review, sizeof(review));

		if(read_size == -1 || read_size == 0) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(7,"Reading Failed!\n");
		}

		if (review != patch.original) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(8, "Error\n");
		}

		lseek(fd3, patch.offset, SEEK_SET);

		if (write(fd3, &patch.modified, sizeof(patch.modified)) != sizeof(patch.modified)) {
			close(fd1);
			close(fd2);
			close(fd3);
			err(9, "Writting Failed\n");
		}
	}
	

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
