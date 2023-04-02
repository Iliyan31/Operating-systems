#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char* argv) {
	if(argc != 3) {
		errx(1, "Invalid number of arguemets!\n");
	}	
	
	int in = open(argv[1], O_RDONLY);
	if(in == -1) {
		err(2,"Open error!\n");
	}

	int out = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (out == -1) {
		close(in);
		err(2, "open error!\n");
	}	

	uint8_t input;
	uint16_t encode = 0;
	ssize_t rs;

	while((rs = read(in, &input, sizeof(input)))) {
		for(int i=0; i<16; i++) {
			// Main logic
		}
		write(1, &encode, sizeof(encode));
		fprintf(stderr, "%d", encode);
		if(write(out, &encode, sizeof(uint16_t))!= sizeof(uint16_t)) {
			close(in);
			close(out);
			err(4, "write error!\n");
		}
	}
	if(rs == -1) {
		close(in);
		close(out);
		err(3, "read error!\n");
	}	

	close(in);
	close(out);
	exit(0);
}	
