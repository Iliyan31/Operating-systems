#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct File {
	uint32_t ID;
	uint16_t saved1;
	uint16_t saved2;
	uint32_t start;
	uint32_t end;
}__attribute__((packed));	

int main(int argc, char* argv[]) {
	if(argc != 2) {
		errx(1, "Inavid arguements count!\n");
	}	
	
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		err(2, "Opening Failed!\n");
	}

	ssize_t rs;
	struct File file;
	uint64_t sum;
	uint32_t counter;
	int index;
	uint32_t users[2048] = {0};
	uint32_t times[2048] = {0};

	while((rs = read(fd, &file, sizeof(file)))) {
		if(rs == -1) {
			close(fd);
			err(3, "Reading Failed!\n");
		}
		
		users[index] = file.ID;
		times[index] = file.end - file.start;
		sum += file.end - file.start;
		counter++;	
		index++;
	}
	
	sum = sum/counter;

	lseek(fd, 0, SEEK_SET);
	uint64_t dis;
	counter = 0;
	
	while((rs = read(fd, &file, sizeof(file)))) {
		if(rs == -1) {
			close(fd);
			err(3, "Reading Failed!\n");
		}
		dis = (file.start - file.end - sum)*(file.start - file.end - sum);
		counter++;
	}
	dis = dis/counter;

	lseek(fd, 0, SEEK_SET);

	while((rs = read(fd, &file, sizeof(file)))) {
		if(rs == -1) {
			close(fd);
			err(3, "reading Failed!\n");
		}

		uint64_t spent = file.end - file.start;
		int max = -1;
		if(spent*spent > dis) {
			for(int i=0; i<index; i++) {
				if(users[i] == file.ID) {
					max = times[i];
				}
			}
		}

		if(write(1, &file.ID, sizeof(file.ID)) != sizeof(file.ID)) {
			close(fd);
			err(4, "Writing Failed!\n");
		}

		if(write(1, &max, sizeof(max)) != sizeof(max)) {
			close(fd);
			err(4, "Writing Failed!\n");
		}
	}
	close(fd);
	exit(0);
}
