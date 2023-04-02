// code goes here

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

const int MAGIC_WORD_SIZE = 3;

struct Instructions {
	uint8_t opcode;
	int64_t operands[3];
}__attribute__((packed));

int main (const int argc, const char* const argv[]) {
	if(argc != 2) {
		errx(1, "Invalud number of arguments\n");
	}
	
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		err(2, "Open Failure!\n");
	}

	struct stat st;
	if(fstat(fd, &st) == -1) {
		err(3, "Stat failed!\n");
	}	
	
	if ((st.st_size - MAGIC_WORD_SIZE*(sizeof(char)) - sizeof(uint32_t)) % 25 !=0) {
		errx(4, "Ivalid file size!\n");
	}

	char str[MAGIC_WORD_SIZE + 1];
	str[MAGIC_WORD_SIZE] = '\0';

	ssize_t rs;
	rs = read(fd, &str, MAGIC_WORD_SIZE);
	if(rs == -1) {	
		err(5, "Reading problem!\n");
	}

	if(rs != MAGIC_WORD_SIZE) {
		errx(29, "Read could not read full magic word!\n");
	}

	if (strcmp(str, "ORC") != 0) {
		errx(6, "The file must begin with ORC!\n");
	}	

	uint32_t ram_size;
	rs = read(fd, &ram_size, sizeof(uint32_t));
	if(rs == -1) {	
		err(5, "Reading problem!\n");
	}

	if(rs != sizeof(uint32_t)) {
		errx(28, "Could not read all ram size!\n");
	}

	if(ram_size == 0) {
		errx(6, "No ram for allocation!\n");
	}

	int64_t* arr = NULL; 
	if((arr = malloc(sizeof(int64_t)*ram_size)) == NULL) {
		free(arr);
		err(7, "Ram allocation problem!\n");
	}

	struct Instructions instr;

	while((rs = read(fd, &instr, sizeof(struct Instructions)))) {
		if(rs == -1) {
			free(arr);
			err(5, "Reading problem!\n");
		}
		if (rs != sizeof(struct Instructions)) {
			free(arr);
			errx(27, "Could not read full struct size!\n");
		}

		switch(instr.opcode) {
			case 0x00: break;
			case 0x95: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(8, "Invalid address in instruction 0x95!\n");
					   }
					   arr[instr.operands[0]] = instr.operands[1];
					   break;

			case 0x5d: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(9, "Invalid address in instruction 0x5d!\n");  
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(9, "Invalid address in instruction 0x5d!\n");
					   }
					   if (arr[instr.operands[1]] >= ram_size || arr[instr.operands[1]] < 0) {
						   free(arr);
						   errx(9, "Invalid address in instruction 0x5d!\n");
					   }

					   arr[instr.operands[0]] = arr[arr[instr.operands[1]]];
					   break;

			case 0x63: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(10, "Invalid address in instruction 0x63!\n");
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(10, "Invalid address in instruction 0x63!\n");	
					   }
					   if (arr[instr.operands[0]] >= ram_size || arr[instr.operands[0]] < 0) {
						   free(arr);
						   errx(10, "Invalid address in instruction 0x63!\n");	
					   }

					   arr[arr[instr.operands[0]]] = arr[instr.operands[1]];
					   break;

			case 0x91: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(11, "Invalid address in instruction 0x91!\n");
					   }
					   if (arr[instr.operands[0]] < 0) {
						   free(arr);
						   errx(12, "Invalid jump count in instruction 0x91!\n");
					   }
						
					   lseek(fd, 7, SEEK_SET);
					   if (lseek(fd, sizeof(struct Instructions)*arr[instr.operands[0]], SEEK_CUR) == -1) {
						   free(arr);
						   err(13, "Lseek problem in instruction 0x91!\n");
					   }

					   break;
			case 0x25: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(14, "Invalid address in instruction 0x25!\n");
					   }
					   if (arr[instr.operands[0]] > 0) {
						   if (lseek(fd, sizeof(struct Instructions), SEEK_CUR) == -1) {
							   free(arr);
							   err(15, "Lseek problem in instruction 0x25!\n");
						   }
					   }
					   break;

			case 0xAD: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(16, "Invalid address in instruction 0xAD!\n");	
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(16, "Invalid address in instruction 0xAD!\n");
					   }
					   if (instr.operands[2] >= ram_size || instr.operands[2] < 0) {
						   free(arr);
						   errx(16, "Invalid address in instruction 0xAD!\n");
					   }

					   arr[instr.operands[0]] = arr[instr.operands[1]] + arr[instr.operands[2]];
					   break;
					  
			case 0x33: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(17, "Invalid address in instruction 0x33!\n");	
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(17, "Invalid address in instruction 0x33!\n");
					   }
					   if (instr.operands[2] >= ram_size || instr.operands[2] < 0) {
						   free(arr);
						   errx(17, "Invalid address in instruction 0x33!\n");		
					   }

					   arr[instr.operands[0]] = arr[instr.operands[1]] * arr[instr.operands[2]];
					   break;

			case 0x04: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(18, "Invalid address in instruction 0x04!\n");	
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(18, "Invalid address in instruction 0x04!\n");	
					   }
					   if (instr.operands[2] >= ram_size || instr.operands[2] < 0) {
						   free(arr);
						   errx(18, "Invalid address in instruction 0x04!\n");	
					   }	
					   if (arr[instr.operands[2]] == 0) {
						   free(arr);
						   errx(19, "Division by zero in instruction 0x04!\n");
					   }

					   arr[instr.operands[0]] = arr[instr.operands[1]] / arr[instr.operands[2]];
					   break;

			case 0xB5: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(20, "Invalid address in instruction 0xB5!\n");	
					   }
					   if (instr.operands[1] >= ram_size || instr.operands[1] < 0) {
						   free(arr);
						   errx(20, "Invalid address in instruction 0xB5!\n");	  
					   }
					   if (instr.operands[2] >= ram_size || instr.operands[2] < 0) {
						   free(arr);
						   errx(20, "Invalid address in instruction 0xB5!\n");	
					   }	
					   if (arr[instr.operands[2]] == 0) {
						   free(arr);
						   errx(21, "Division by zero in instruction 0xB5!\n");
					   }

					   arr[instr.operands[0]] = arr[instr.operands[1]] % arr[instr.operands[2]];
					   break;

			case 0xC1: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(22, "Invalid address in instruction 0xC1!\n");	
					   }

					   char a = (char)(arr[instr.operands[0]]);
					   if (write(1, &a, sizeof(a)) != sizeof(char)) {
						   free(arr);
						   err(23, "Writing problem in instruction 0xC1!\n");
					   }
					   break;

			case 0xBF: if (instr.operands[0] >= ram_size || instr.operands[0] < 0) {
						   free(arr);
						   errx(24, "Invalid address in instruction 0xBF!\n");	
					   }
					   if (arr[instr.operands[0]] < 0) {
						   free(arr);
						   errx(25, "Sleep time can't be below 0 in instruction 0xBF!\n");
					   }

					   sleep((arr[instr.operands[0]]/1000)%60);
					   break;

			  default: free(arr);
					   errx(26, "Invalid Instruction passed to the program!\n");
		}		
	}	

	free(arr);
	close(fd);	
	exit(0);
}

