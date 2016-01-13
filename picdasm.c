/* PIC Disassembler. Initially for the 16F84 and compatibles, others to
 * follow.
 * 
 * NOTE: currently only the function to map instructions to strings are
 * given. There is nothing yet for parsing object formats like Intel INHX8M.
 * 
 * Written by Owen Klan  -  March 2003
 */

/* UPDATE HISTORY
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "picdasm.h"

/* Returns version information as an integer */
int pdasm_get_version()
{
    static unsigned char major = 0;
    static unsigned char minor = 1;
    
    return ((major << 8) & minor);
}

int main(int argc, char *argv[])
{
    unsigned short instr;
    int offset = 0;
    int i = 0;
    char *in;		       /* Instruction string */
    char *image;		       /* Mmapped pointer */
    int fd = 0;
    struct stat fs;
    
    if (argc < 2) {
	fprintf(stderr, "USAGE:  picdasm file\n");
	fprintf(stderr, "Disassemble a PIC-16F84 binary file\n");
	return 1;
    }
    
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
	fprintf(stderr, "Failed opening %s! %s\n", argv[1], strerror(errno));
	return 1;
    }
    
    fstat(fd, &fs);
    
    /* Now mmap() it. */
    if ((image = mmap(image, fs.st_size, PROT_READ,
		      MAP_SHARED, fd, 0)) == NULL) {
	fprintf(stderr, "Failed mapping %s into memory! %s\n", argv[1],
		strerror(errno));
	close(fd);
	return 1;
    }

    for (i = 0; i < (fs.st_size / 2); i++, offset += 2) {
	instr = *(unsigned short *)(image + offset);
	in = disassemble_16f84(instr, FSR_NAMES);
	if (!in)
	  continue;
	
	fprintf(stdout, "%04X  %04X\t%s\n", i, instr, in);
	free(in);
    }

    munmap(image, fs.st_size);
    close(fd);
    
    return 0;
}
