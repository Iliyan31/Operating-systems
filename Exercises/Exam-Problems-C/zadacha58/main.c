#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int cmp(const void* a, const void* b) {
	if(*((uint32_t*)a) < *((uint32_t*)b)) {
		return -1;
	}	
	else if (*((uint32_t*)a) < (*(uint32_t*)b)) {
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Not enough arguements!\n");
	}

	struct stat st;
	if (stat(argv[1], &st) == -1) {
		err(2, "Failure!\n");
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		err(3, "Opening failure!\n");
	}

	uint32_t size = st.st_size / sizeof(uint32_t);
	if (size > 4194304) {
		errx(4, "File is too large!\n");
	}

	ssize_t readSize;
	uint32_t halfSize = size/2;
	uint32_t* arr = malloc(halfSize* sizeof(uint32_t));
	
	if((readSize = read(fd, arr, sizeof(halfSize* sizeof(uint32_t))))) {
		if(readSize == -1) {
			close(fd);
			err(5, "Reading failure!\n");
		}
	}


	int temp = open("temp.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(temp == -1) {
		close(fd);
		err(6, ",,,,,\n");
	}
	
	if(write(temp, arr, halfSize* sizeof(uint32_t))!= halfSize* sizeof(uint32_t)) {
		close(fd);
		close(temp);
		err(2, "1111\n");
	}

	free(arr);

	uint32_t otherHalf = size - halfSize;
	arr = malloc(otherHalf*sizeof(uint32_t));

	if((readSize = read(fd, arr, otherHalf*sizeof(uint32_t)))) {
		if(readSize == -1) {
			close(fd);
			close(temp);
			err(2, "22222\n");
		}
	}

	qsort(arr, otherHalf, sizeof(uint32_t), cmp);

	int temp2 = open("temp2.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(temp2 == -1) {
		close(fd);
		close(temp);
		err(6, ",,,,,\n");
	}

	if(write(temp2, arr, otherHalf*sizeof(uint32_t)) != otherHalf*sizeof(uint32_t)) {
		close(fd);
		close(temp);
		close(temp2);
		err(6, ",,,,,\n");
	}

	int sorted = open(argv[2],O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
		
	lseek(temp, 0, SEEK_SET);
	lseek(temp2, 0, SEEK_SET);

	ssize_t rs1, rs2;
	uint32_t a;
  	uint32_t b;
	
	while((rs1 = read(temp, &a, sizeof(a))) && (rs2 = read(temp2, &b, sizeof(b)))) {
		if(a <= b) {
			if (write(sorted, &a, sizeof(a)) != sizeof(a)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, ",,,,,\n");
			}
			lseek(temp2, -1*sizeof(b), SEEK_CUR);
		}
		else {
			if (write(sorted, &b, sizeof(b)) != sizeof(b)) {
				close(fd);
				close(temp);
				close(temp2);
				close(sorted);
				err(6, ",,,,,\n");
			}
			lseek(temp, -1*sizeof(a), SEEK_CUR);
		}
	}
	if(rs1 == -1 || rs2 == -1) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, ",,,,,\n");
	}
	
	if(rs1 == sizeof(a)) {
		if(write(sorted, &a, sizeof(a)) != sizeof(a)) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, ",,,,,\n");	
		}
	}

	while((rs2 = read(temp2, &b, sizeof(b)))) {
		if(write(sorted, &b, sizeof(b)) != sizeof(b)) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, ",,,,,\n");	
		}
	}

	if(rs2 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(6, ",,,,,\n");
	}	

	while((rs1 = read(temp, &a, sizeof(a)))) {
		if(write(sorted, &a, sizeof(a)) != sizeof(a)) {
			close(fd);
			close(temp);
			close(temp2);
			close(sorted);
			err(6, ",,,,,\n");	
		}
	}

	if(rs1 == -1) {
		close(fd);
		close(temp);
		close(temp2);
		close(sorted);
		err(6, ",,,,,\n");
	}

	unlink("temp.txt");
	unlink("temp2.txt");

	
	close(fd);
	close(temp);
	close(temp2);
	close(sorted);
	
	exit(0);
}
