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
	ssize_t read_size;
	char buffer;
	bool flag = false;

	if (strcmp(argv[1], "-d") == 0 && argc == 3) {
		char* toDelete = malloc(strlen(argv[2]+1));
		strcpy(toDelete, argv[2]);

		while((read_size == read(0, &buffer, sizeof(buffer)))) {
			if(read_size == -1) {
				err(1, "Reading Error!\n");
			}

			for(int i=0; i<strlen(toDelete); i++) {
				if (buffer == toDelete[i]) {
					flag = true;
					break;
				}
			}
		}
		free(toDelete);
	}


	exit(0);
}	
