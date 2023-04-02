#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>

int main(const int argc, const char* const argv[]) {
	if (argc != 2) {
		errx(1, "NUmber!\n");
	}	

	int fd = open("74-fifo", O_RDONLY);
	if (fd == -1) {
		err(2, "Open problem!\n");
	}

	dup2(fd, 0);

	if(execlp(argv[1], argv[1], (char*)NULL) == -1) {
		err(3, "Execlp!\n");
	}	

	exit(0);
}
