#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HDR_LEN 20

int main(int argc, char *argv[]) {
	 if (argc != 2) {
	  fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	FILE * fp = fopen(argv[1], "wb");
	if (!fp) {
		fprintf(stderr, "ERROR: Unable to open output file.\n");
		exit(1);
	}
	char buff[HDR_LEN] = {0x45, 0x00, 0x00, 0x44,
					0xad, 0x0b, 0x00, 0x00,
					0x40, 0x11, 0x72, 0x72,
					0xac, 0x14, 0x02, 0xfd,
					0xac, 0x14, 0x00, 0x06};
	fwrite(buff, 1, HDR_LEN, fp);
	return 0;
}

