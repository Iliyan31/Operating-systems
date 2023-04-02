/*#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

struct Header {
	uint32_t magic;
	uint8_t headerVersion;
	uint8_t dataVersion;
	uint16_t count;
	uint32_t reserved1;
	uint32_t reserved2;
};

struct Version1 {
	uint16_t offset;
	uint8_t original;
	uint8_t modified;
};

struct Version2 {
	uint32_t offset;
	uint16_t original;
	uint16_t modified;
};


int main(int argc, char* argv[]) {
	if(argc != 4) {
		errx(1, "There must be 4 parameters!\n");
	}

	int p = open(argv[1], O_RDONLY);
	if(p == -1) {
		err(2, "Opening failed!\n"); 
	}

	int f1 = open(argv[2], O_RDONLY);
	if(f1 == -1) {
		close(p);
		err(2, "Opening failed!\n");
	}

	struct Header h;
   	ssize_t rs = read(p, &h, sizeof(h));
	if(rs == -1) {
		close(p);
		close(f1);
		err(3,"Reading Failed!\n");
	}	

	if(h.magic != 0xEFBEADDE) {
		close(p);
		close(f1);
		errx(5, "Magic key is not right!\n");
	}

	if (h.headerVersion != 0x01) {
		close(p);
		close(f1);
		errx(6, "Version not right!");
	}

	int f2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(f2 == -1) {
		close(p);
		close(f1);
		err(2, "Opening failed!\n");
	}
	
	if(h.dataVersion == 0x00) {
		uint8_t buffer;
		while((rs = read(f1,&buffer, sizeof(buffer)))) {
			if (rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			if(write(f2, &buffer, sizeof(buffer)) != sizeof(buffer)) {
				close(p);
				close(f1);
				close(f2);
				err(4,"Writing Failed!\n");
			}

		}

		struct Version1 v1;

		while((rs = read(p, &v1, sizeof(v1)))) {
			if(rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			
			if(lseek(f1, 0, SEEK_SET) == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			rs = read(f1, &buffer, sizeof(buffer));
			if(rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			
			if(buffer == v1.original) {
				lseek(f2, v1.offset, SEEK_SET);
				if(write(f2, &v1.modified, sizeof(v1.modified)) != sizeof(v1.modified)) {
					close(p);
					close(f1);
					close(f2);
					err(4,"Writing Failed!\n");
				}
			}
			else{
				close(p);
				close(f1);
				close(f2);
				err(4,"No such byte in this offset!\n");
			}
		}

	}
	else if(h.dataVersion == 0x01) {
		uint16_t buffer;
		while((rs = read(f1,&buffer, sizeof(buffer)))) {
			if (rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			if(write(f2, &buffer, sizeof(buffer)) != sizeof(buffer)) {
				close(p);
				close(f1);
				close(f2);
				err(4,"Writing Failed!\n");
			}

		}
		struct Version2 v2;

		while((rs = read(p, &v2, sizeof(v2)))) {
			if(rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			
			if(lseek(f1, 0, SEEK_SET) == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			rs = read(f1, &buffer, sizeof(buffer));
			if(rs == -1) {
				close(p);
				close(f1);
				close(f2);
				err(3,"Reading Failed!\n");
			}
			
			if(buffer == v2.original) {
				lseek(f2, v2.offset, SEEK_SET);
				if(write(f2, &v2.modified, sizeof(v2.modified)) != sizeof(v2.modified)) {
					close(p);
					close(f1);
					close(f2);
					err(4,"Writing Failed!\n");
				}
			}
			else{
				close(p);
				close(f1);
				close(f2);
				err(4,"No such byte in this offset!\n");
			}
		}
	}
	else {
		close(p);
		close(f1);
		close(f2);
		errx(6, "Invalid Version!\n");
	}

	close(p);
	close(f1);
	close(f2);	

	exit(0);
}
*/




#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <string.h>

struct Header {
	uint32_t magic;
	uint8_t headerVers;   
	uint8_t dataVers;
	uint16_t count;
	uint32_t reserved1;
	uint32_t reserved2;
}__attribute__((packed));

struct Version1 {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed));

struct Version2 {
	uint32_t offset;
	uint16_t original;
	uint16_t new;
}__attribute__((packed));

void closefd(int fd) {
	int n = errno;
	if(close(fd) == -1) {
		err(15, "kdafb!\n");
	}
	errno = n;
}

int main (const int argc, const char* const argv[]) {
	if (argc != 4) {
		errx(1, "skdhfb!\n");
	}

	int fdp = open(argv[1], O_RDONLY);
	if (fdp == -1) {
		err(2, "Open!\n");
	}

	int fd1 = open(argv[2], O_RDONLY);
	if (fd1 == -1) {
		err(2, "Open!\n");
	}

	int fd2 = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd2 == -1) {
		err(2, "Open!\n");
	}
	
	struct Header head;
	if (read(fdp, &head, sizeof(struct Header)) != sizeof(struct Header)) {
		err(3, "Read!\n");
	}

	if (head.magic != 0xEFBEADDE) {
		errx(4, "Wrong file type!\n");
	}

	if (head.headerVers != 0x01) {
		errx(5, "slkng!\n");
	}

	if(head.dataVers != 0x01 && head.dataVers != 0x00){
		errx(5,"Wrong data version");
	}
	
	int v = 0;
	if (head.dataVers == 0x01) {
		v = 1;
	}
	
	struct stat st1;
	if (fstat(fdp, &st1) == -1) {
		err(6, "stat err!\n");
	}
	
	struct stat st2;
	if (fstat(fd1, &st2) == -1) {
		err(6, "stat err!\n");
	}
	
	if (v == 1) {
		if (st2.st_size % sizeof(uint32_t) != 0) {
			errx(8, "AS:VLM!\n");
		}
		
		if ((st1.st_size - sizeof(struct Header)) % sizeof(struct Version2) !=0 ) {
			errx(9, "SLKNVc!\n");
		}
	}
	else {
		if (st2.st_size % sizeof(uint16_t) != 0) {
			errx(8, "AS:VLM!\n");
		}
		
		if ((st1.st_size - sizeof(struct Header)) % sizeof(struct Version1) !=0 ) {
			errx(9, "SLKNVc!\n");
		}
	}

	

	exit(0);
}
