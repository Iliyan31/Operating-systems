#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char* argv[]) {
	char buffer;
	ssize_t read_size;

	for(int i=1; i<=argc; i++) {
		if(strcmp(argv[i], "-") == 0) {
			while((read_size = read(0, &buffer, sizeof(buffer)))) {
				if(read_size == -1) {
					err(1, "Reading Error!\n");
				}

				fprintf(stdout, "%c", buffer);
			}
			continue;
		}

		int fd = open(argv[i], O_RDONLY);
		if(fd == -1) {
			err(2, "Opening Error!\n");
		}

		while((read_size == read(fd, &buffer, sizeof(buffer)))) {
			if (read_size == -1) {
				err(3, "Reading Error!\n");
			}
			fprintf(stdout, "%c", buffer);
		}
		close(fd);
	}
	exit(0);
}
