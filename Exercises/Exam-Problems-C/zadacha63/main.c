#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct Interval {
	uint16_t offset;
	uint16_t len;
};

struct Header {
	uint8_t arr[8];
	uint16_t count;
	uint8_t arr2[10];
}__attribute__((packed));

int main(const int argc, const char* const argv[]) {
	if(argc != 7) {
		errx(1, "Number of arguements!\n");
	}
	int f[4];

	int sizes[4];
	sizes[0] = 4;
	sizes[1] = 1;
	sizes[2] = 2;
	sizes[3] = 8;

	for(int i = 2; i < (argc - 1); i++) {
		f[i-2] = open(argv[i], O_RDONLY);
		if(f[i-2] == -1) {
			err(2, "Open failure!\n");
		}
	}

	int fdaffix = open(argv[1], O_RDONLY);
	if(fdaffix == -1) {
		err(2, "Open failure!\n");
	}

	int fdcrus = open(argv[7], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fdcrus == -1) {
		err(2, "Open failure!\n");
	}
	
	struct Interval k[4];
	uint64_t aff;
	ssize_t rs;

	while((rs = read(fdaffix, &k, sizeof(k)))) {
		if(rs == -1 || rs != sizeof(k)) {
			err(3, "Read failed!\n");
		}

		for (int i=0; i<4; i++) {
			if (lseek(f[i], (k[i].offset+16)*sizes[i], SEEK_SET) == -1) {
				err(4, "Lseek problem!\n");
			}
			for(int j=0; j<k[i].len; j++) {
				if (read(f[i], &aff, sizes[i]) != sizes[i]) {
					err(3, "Read failed!\n");
				}
				if(write(fdcrus, &aff, sizes[i]) != sizes[i]) {
					err(4, "write failed!\n");
				}
			}
		}
	}

	for(int i=0; i<4; i++) {
		if(close(f[i]) == -1) {
			err(5, "Close failed!\n");
		}
	}

	if(close(fdaffix) == -1) {
		err(5, "Close failed!\n");
	}

	if(close(fdcrus) == -1) {
		err(5, "Close failed!\n");
	}

	exit(0);
}
