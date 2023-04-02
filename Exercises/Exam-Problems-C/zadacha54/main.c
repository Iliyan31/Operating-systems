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
	if(argc == 1) {
		char c;
		ssize_t rs;
		while((rs = read(0, &c, sizeof(c)))) {
			if (rs == -1 || rs != sizeof(char)) {
				err(2, "read error!\n");
			}
			if(write(1, &c, sizeof(char))!= sizeof(char)) {
				err(1, "write error!\n");
			}
		}
	}
	else {
		if(strcmp(argv[1], "-n") == 0) {
			char c;
			ssize_t rs;
			int newline = 1;
			int line = 1;
			for(int i=2; i<=argc; i++) {
				if(strcmp(argv[i], "-") == 0)  {
					while((rs = read(0, &c, sizeof(c)))) {
						if(newline) {
							printf("%d", line);
						}
						if(write(1, &c, sizeof(c))!=sizeof(c)) {
							err(1, "write error!\n");
						}
						if(c == '\n') {
							newline = 1;
						   	line++;	
						}
						else {
							newline = 0;
						}
					}
					if(rs == -1) {
						err(2, "read error!\n");
					}
				}
				
				int fd = open(argv[i], O_RDONLY);
		   		if(fd == -1) {
					err(3, "open error!\n");
				}
				while((rs = read(fd, &c, sizeof(c)))) {
					if(newline) {
						printf("%d", line);
					}
					if(write(1, &c, sizeof(c))!=sizeof(c)) {
						err(1, "write error!\n");
					}
					if(c == '\n') {
						newline = 1;
					   	line++;	
					}
					else {
						newline = 0;
					}
				}
				if(rs == -1) {
					err(2, "read error!\n");
				}
				close(fd);		
			}
		}
		else {
			char c;
			ssize_t rs;
			for(int i=1; i<argc; i++) {
				if(strcmp(argv[i], "-") == 0) {
					while((rs = read(0, &c, sizeof(c)))) {
						if(write(1, &c, sizeof(c))!=sizeof(c)) {
							err(1, "write error!\n");
						}
					}
					if(rs == -1) {
						err(2, "read error!\n");
					}
				}

				int fd = open(argv[i], O_RDONLY);
				if(fd == -1) {
					err(3, "open error!\n");
				}
				while((rs = read(0, &c, sizeof(c)))) {
						if(write(1, &c, sizeof(c))!=sizeof(c)) {
							err(1, "write error!\n");
						}
				}
				if(rs == -1) {
					err(2, "read error!\n");
				}
				close(fd);
			}
		}	
	}


	
	exit(0);
}
