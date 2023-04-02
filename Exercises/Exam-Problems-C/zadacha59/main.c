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
	char c;
	ssize_t rs;

	if(argc == 3 && strcmp(argv[1], "-c") == 0) {
		if (strlen(argv[2]) == 1) {
			int position = argv[2][0] - '0';
			int counter = 1;
			while((rs = read(0, &c, sizeof(c)))) {
				if (rs == -1) {
					err(1, "Redaing Failed!\n");
				}	
				if (counter == position) {
					if(write(1, &c, sizeof(c)) != sizeof(c)) {
						err(2, "Writting Failed!\n");
					}
				}
				counter++;
			}
		}
		else if(strlen(argv[2]) == 3) {
			int from = argv[2][0] - '0';
			int to = argv[2][2] - '0';
			int counter = 1;

			if(from > to) {
				errx(3, "Wrong Arguements!\n");
			}
			while((rs = read(0, &c, sizeof(c)))) {
				if (rs == -1) {
					err(1, "Redaing Failed!\n");
				}	
				if (counter >= from && counter <= to) {
					if(write(1, &c, sizeof(c)) != sizeof(c)) {
						err(2, "Writting Failed!\n");
					}
				}
				counter++;
			}
		}
	}
	else if (strcmp(argv[1], "-d") == 0 && strcmp(argv[3], "-f") == 0 && argc == 5) {
		
		if (strlen(argv[4]) == 1) {
			int counter = 1;
			int position = argv[4][0] - '0';
			char delimiter = argv[2][0] - '0';

			while((rs = read(0, &c, sizeof(c)))) {
				if(rs == -1) {
					err(1, "Reading Failed!\n");
				}

				if (delimiter == c) {
					counter++;
					continue;
				}
				if (position == counter) {
					if(write(1, &c, sizeof(c))!=sizeof(c)) {
						err(2, "Writting Failed!\n");
					}
				}
			}
		}
		else if (strlen(argv[4]) == 3)  {
			int counter = 1;
			int from = argv[4][0] - '0';
			int to = argv[4][3] - '0';
			char delimiter = argv[2][0] - '0';
			
			if(from > to) {
				err(3, "Wrong Arguements!\n");
			}

			while((rs = read(0, &c, sizeof(c)))) {
				if(rs == -1) {
					err(3, "Wrong Arguements!\n");
				}

				if (delimiter == c) {
					counter++;
					continue;
				}
				if (counter >= from && counter <= to) {
					if(write(1, &c, sizeof(c))!=sizeof(c)) {
						err(2, "Writting Failed!\n");
					}
				}
			}	
		}	
	}

	exit(0);
}
