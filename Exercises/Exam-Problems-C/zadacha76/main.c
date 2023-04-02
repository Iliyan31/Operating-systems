#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct Three {
	char fileName[8];
	uint32_t offset;
	uint32_t length;
}__attribute__((packed));

int main(const int argc, const char* const argv[]) {
	if (argc != 2) {
		errx(1, "bdhjb!\n");
	}	
	
	struct stat st;
	if(stat(argv[1], &st) == -1){
		err(3, "shafhjdwb!\n");
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		err(2, "kszjkvjkjsc!\n");
	}

	if(st.st_size > 8*sizeof(struct Three)) {
		errx(4, "lsjflkn!1\n");
	}	
	
	if(st.st_size % sizeof(struct Three) != 0) {
		errx(5, ".....!\n");
	}
		
	int size = st.st_size/sizeof(struct Three);
	
	int a[2];
	if (pipe(a) == -1) {
		err(6, "akjfbk!\n");
	}

	pid_t p;
	for (int i=0; i<size ; i++) {
		p = fork();
		if (p == -1) {
			err(7, "jdcjWD!\n");
		}
		if (p == 0) {
			break;
		}
	}

	if (p == 0) {
		close(a[0]);
		uint16_t temp = 0;
		struct Three three;
		if (read(fd, &three, sizeof(struct Three)) != sizeof(struct Three)) {
			err(8, "ksjfgjk!\n");
		}		

		int fd1 = open(three.fileName, O_RDONLY);
		if(fd1 == -1) {
			err(9, "kjsghdfk!\n");
		}

		if (lseek(fd1, three.offset*sizeof(uint16_t), SEEK_SET) == -1) {
			err(10, "Lseek!\n");
		}

		for(uint32_t i=0; i<three.length; i++) {
			uint16_t num;
			if(read(fd1, &num, sizeof(uint16_t)) != sizeof(uint16_t)) {
				err(11, "dlfnn1!\n");
			}
			temp^=num;
		}
		if (write(a[1], &temp, sizeof(uint16_t))!= sizeof(uint16_t)) {
			err(12,"kzhjfjbawd!1\n");
		}

		exit(0);
	}
	close(a[1]);
	uint16_t sum = 0;
	for(int i=0; i<size; i++) {
		uint16_t temp; 
		if(read(a[0], &temp, sizeof(uint16_t)) != sizeof(uint16_t)){
			err(13, "akjfsgjkj!\n");
		}
		sum^=temp;	
	}
	fprintf(stdout, "%04x\n", sum);
	exit(0);
}
