#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <elf.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "usage: dump <file.o>\n");
	exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    assert(fd >= 0);

    // read in elf header
    Elf32_Ehdr elf;
    int rc = pread(fd, &elf, sizeof(Elf32_Ehdr), 0);
    assert(rc == sizeof(elf));
    assert(elf.e_ident[0] == ELFMAG0);
    assert(elf.e_ident[1] == ELFMAG1);
    assert(elf.e_ident[2] == ELFMAG2);
    assert(elf.e_ident[3] == ELFMAG3);

    unsigned int off;
    Elf32_Shdr sh;

    int i;
    for (i = 0, off = elf.e_shoff; i < elf.e_shnum; i++, off += sizeof(sh)) {
        rc = pread(fd, &sh, sizeof(sh), off);
        assert(rc == sizeof(sh));

	// look for code (SHT_PROGBITS) that has a non-zero size: a hack but good enough for now
	if (sh.sh_type == SHT_PROGBITS && sh.sh_size > 0) {

	    // now make space on heap for entire code segment
	    unsigned char *buffer = malloc(sh.sh_size);
	    assert(buffer != NULL);

	    // read segment into buffer
	    rc = pread(fd, buffer, sh.sh_size, sh.sh_offset);
	    assert(rc == sh.sh_size);

	    // starting address
	    printf("starting address: %x\n", sh.sh_addr);

	    // print out each byte (in hex)
	    unsigned char *b;
	    for (b = buffer; b < buffer + sh.sh_size; b++)
		printf("%x ", *b);
	    printf("\n");

	    // when we've done this once, just break out of loop -- 
	    // all done with this .o
	    break;
	}
    }

    close(fd);

    return 0;
}
