#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

int main(const int argc, const char* const argv[]) {
	if (argc > 1) {
		err(1, "Number of arguements!\n");
	}		
	else if(argc == 0) {
		
	}
	else if(argc == 1) {
		if(strlen(argv[1]) > 4) {
			errx(2, "Size over 4!\n");
		}

	}

	
	exit(0);
}
