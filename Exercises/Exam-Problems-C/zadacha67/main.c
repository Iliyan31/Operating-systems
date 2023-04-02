#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if(argc != 3) {
		errx(1, "Number of arguements!\n");
	}
	
	int in = open(argv[1], O_RDONLY);
	if(in == -1) {
		err(2, "Open failed!\n");
	}

	int out = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(out == -1) {
		err(2, "Open failed!\n");
	}
	
	struct stat st;
	if(stat(argv[1], &st) == -1) {
		err(5, "Stat failed!\n");
	}

	uint32_t count = (st.st_size/sizeof(uint16_t));
	uint16_t buff;
	ssize_t rs;
	const char s[] = "const uint32_t arrN = ";
	const char str[] = "const uint16_t arr[] = {";
	
	if(write(out, &s, (int)(sizeof(char)*strlen(s))) != (int)(sizeof(char)*strlen(s))) {
		err(4, "Write failed!\n");
	}

	if(write(out, &count, sizeof(uint32_t)) != sizeof(uint32_t)) {
		err(4, "Write failed!\n");
	}

	if(write(out, ";\n", 2) != 2) {
		err(4, "Write failed!\n");
	}

	if(write(out, &str, (int)(strlen(str)*sizeof(char))) != (int)(strlen(str)*sizeof(char))) {
		err(4, "Write failed!\n");
	}

	while((rs = read(in, &buff, sizeof(uint16_t)))) {
		if(rs == -1 || rs != sizeof(uint16_t)) {
			err(3, "Read failed!\n");
		}
		if(write(out, &buff, sizeof(uint16_t)) != sizeof(uint16_t)) {
			err(4, "Write failed!\n");
		}
		if(write(out,",", 1) != 1) {
			err(4, "Write failed!\n");
		}
	}	
	if(write(out,"};", 2) != 2) {
			err(4, "Write failed!\n");
	}

	if(close(in) == -1) {
		err(6, "Close failed!\n");
	}

	if(close(out) == -1) {
		err(6, "Close failed!\n");
	}

	exit(0);
}
