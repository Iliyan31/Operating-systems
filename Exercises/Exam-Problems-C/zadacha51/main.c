#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int cmp(const void* a, const void* b) {
	if(*((uint32_t*)a) > *((uint32_t*)b)) {
		return 1;
	}
	else if(*((uint32_t*)a) > *((uint32_t*)b)) {
		return -1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		errx(1,"Invalid number of arguements!\n");
	}

	struct stat st;
	if(stat(argv[1], &st) == 1) {
		err(2, "Stat error!\n");
	}

	int32_t size = st.st_size / sizeof(uint32_t);
	if(size == 0) {
		errx(3, "File is empty!\n");
	}

	if (size > 100000000) {
		errx(7, "The limit of numbers in the file is 100 000 000!\n");
	}

	uint32_t hsize = size/2;
	uint32_t buffer = malloc(hsize*sizeof(uint32_t));

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		free(buffer);
		err(4,"Read error!\n");
	}

	ssize_t rs = read(fd, buffer, sizeof(buffer));
	if(rs == -1) {
		free(buffer);
		close(fd);
		err(5, "read error!\n");
	}

	int temp = open("temp.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(temp == -1) {
		free(buffer);
		close(fd)
		err(4,"Open error!\n");
	}
	qsort(buffer, hsize, sizeof(uint32_t), cmp);

	if(write(temp, &buffer, sizeof(buffer))!= sizeof(buffer)) {
		free(buffer);
		close(fd);
		close(temp);
		err(6, "write error!\n");
	}
	free(buffer);

	rs = 0;
	buffer = malloc(hsize*sizeof(uint32_t));
	rs = read(fd, &buffer, sizeof(buffer));
	if(rs == -1) {
		free(buffer);
		close(fd);
		close(temp);
		err(6, "Read error!\n");
	}	

	int temp2 = open("temp2.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (temp2 == -1) {
		free(buffer);
		close(fd);
		close(temp);
		err(6, "open error!\n");
	}
	
	qsort(buffer, hsize, sizeof(uint32_t), cmp);

	if(write(temp2, buffer, sizeof(buffer))!= sizeof(buffer)) {
		free(buffer);
		close(fd);
		close(temp);
		err(6, "write error!\n");
	}

	free(buffer);
	
	lseek(temp, 0, SEEK_SET);
	lseek(temp2, 0, SEEK_SET);

	int sorted = open("sorted.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(sorted == -1) {
		close(fd);
		close(temp);
		close(temp2);
		err(4, "open error!\n");
	}

	ssize_t rs1, rs2;
	uint32_t a, b;
	
	while((rs1 = read(temp, &a, sizeof(a))) && (rs2 = read(temp2, &b, sizeof(b)))) {
		if(a <= b) {
			if(write(sorted, &a, sizeof(a))!= sizeof(a)) {
				int n = errno;
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				ernno = n;
				err(6, "write error!\n");
			}
			lseek(temp2, -1*sizeof(b), SEEK_CUR);	

		}
		else {
			if(write(sorted, &b, sizeof(b))!=sizeof(b)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "write error!\n");
			}
			lseek(temp, -1*sizeof(a), SEEK_CUR);
		}
	}

	if(rs1 == -1 || rs2 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(5, "read error!\n");
	}

	if (rs1 == sizeof(a)) {
		if(write(sorted, &a, sizeof(a))!=sizeof(a)) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, "write error!\n");
		}
	}

	if(rs1 == 0) {
		while((rs2 = read(temp2, &b, sizeof(b)))) {
			if(write(sorted, &b, sizeof(b))!=sizeof(b)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "write error!\n");
			}
		}
	}
	else if(rs2 == 0) {
		while((rs1 = read(temp, &a, sizeof(a)))) {
			if(write(sorted, &a, sizeof(a))!=sizeof(a)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, "write error!\n");
			}
		}
	}

	if(rs1 == -1 || rs2 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(5, "read error!\n");
	};

	unlink("temp.txt");
	unlink("temp2.txt");

	close(fd);
	close(temp);
	close(temp2);
	exit(0);
}
