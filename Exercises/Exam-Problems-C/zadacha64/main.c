#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct SDL {
	uint16_t arr[8];
}__attribute__((packed));

const int bytes = 8;

static int cmp(const void* a, const void* b) {
	return (*(const uint32_t*)a) > (*(const uint32_t*)b) ? 1 : -1;
}

int main(const int argc, const char* const argv[]) {
	if (argc !=3) {
		errx(1, "Number of arguements!\n");
	}

	int scl = open(argv[1], O_RDONLY);
	if(scl == -1) {
		err(2, "Open failed!\n");
	}

	int sdl = open(argv[2], O_RDONLY);
	if(sdl == -1) {
		err(2, "Open failed!\n");
	}

	
	struct stat st;
	if(stat(argv[1], &st) == -1) {
		err(5, "Stat failed!\n");
	}

	struct stat st2;
	if(stat(argv[2], &st2) == -1) {
		err(5, "Stat failed!\n");
	}

	if((uint32_t)(st.st_size * bytes) != (uint32_t)(st2.st_size / sizeof(uint16_t))) {
		errx(6, "Files does not match size!\n");
	}

	int fd = open("temp", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		err(2, "Open failed!\n");
	}
	
	ssize_t rs, rs2;
	char c;
	struct SDL f;
	while((rs = read(scl, &c, sizeof(char))) && (rs2 = read(sdl, &f, sizeof(struct SDL)))) {
		if(rs == -1 || rs != sizeof(char) || rs2 == -1 || rs2 != sizeof(struct SDL)) {
			err(3, "Read failed!\n");
		}
		
		uint16_t temp;
		for(int i = 7; i >= 0; i++) {
			temp = ((c >> i) & 0x01);
			if (temp == 1) {
				if(write(fd, &f.arr[i], sizeof(uint16_t)) != sizeof(uint16_t))	{
					err(4, "Write failed!\n");
				}	
			}
		}
	}

	if(close(scl) == -1) {
		err(7, "Close failed!\n");
	}

	if(close(sdl) == -1) {
		err(7, "Close failed!\n");
	}

	if(close(fd) == -1) {
		err(7, "Close failed!\n");
	}

	exit(0);
}
