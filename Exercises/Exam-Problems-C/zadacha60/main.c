#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>


static int cmp(const void* a, const void* b) {
	return (*(const uint16_t*)a) > (*(const uint16_t*)b) ? 1 : -1;
}

int main(const int argc, const char* const argv[]) {
	if (argc != 3) {
		errx(1, "Number of arguements!\n");
	}

	struct stat st;
	if (stat(argv[1], &st) == -1) {
		err(2, "Stat error!\n");
	}

	int size = st.st_size/sizeof(uint16_t);

	if(size > 65535) {
		errx(3, "Size over the limit!\n");
	}

	int in = open(argv[1], O_RDONLY);
	if(in == -1) {
		err(4, "In open failed!\n");
	}

	int out = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(out == -1) {
		err(5, "Out open failed!\n");
	}
	
	uint16_t* arr = malloc(size*sizeof(uint16_t) + 1);
	if(arr == NULL) {
		free(arr);
		err(6, "No memory allocated!\n");
	}
	
	if(read(in, arr, (uint32_t)(sizeof(uint16_t)*size)) != (uint32_t)(sizeof(uint16_t)*size)){
		free(arr);
		err(7, "Read arr from in failed!\n");
	}	

	qsort(arr, size, sizeof(uint16_t), cmp);

	if(write(out, arr, (uint32_t)(sizeof(uint16_t)*size)) != (uint32_t)(sizeof(uint16_t)*size)) {
		free(arr);
		err(8, "write in out from arr failed!\n");
	}

	free(arr);
	if(close(in) == -1) {
		err(9, "Unable to close in!\n");
	}
 	if(close(out) == -1) {
		err(9, "Unable to close out!\n");
	}

	exit(0);
}
