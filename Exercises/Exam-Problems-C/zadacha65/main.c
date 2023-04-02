#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <math.h>

uint16_t myPow(uint16_t num, uint16_t degree) {
	if(degree == 0) return 1;
	uint16_t sum = 1;
	for(int i=0; i<degree; i++) {
		sum*=num;
	}
	return sum;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "Invalid number of arguements!\n");
	}

	int in = open(argv[1], O_RDONLY);
	if(in == -1) {
		err(2, "Open failure!\n");
	}

	int out = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(out == -1) {
		close(in);
		err(2, "Open failure!\n");
	}

	uint8_t input;
	uint16_t encode;
	ssize_t rs;

	while((rs = read(in, &input, sizeof(input)))) {
		for(int i=0; i<8; i++) {
			// знаем че на дадената позиция има 1
			if(input & (1 << i)) {
				//////////	
			}
			else {
				/////////
			}
		}

		if (write(1, &encode, sizeof(encode)) != sizeof(encode)) {
			continue;
		}

		fprintf(stderr, "%d", encode);
		if(write(out, &encode, sizeof(encode))!=sizeof(encode)) {
			close(in);
			close(out);
			err(3, "Write error!\n");
		}
	}
	if(rs == -1) {
		close(in);
		close(out);
		err(3, "read failure!\n");
	}

	close(in);
	close(out);
	exit(0);

