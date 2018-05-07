#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#define CS3_BASE   0x0C000000
#define SEG1_OFFSET       0x0600
#define SEG2_OFFSET       0x0700
#define SEG3_OFFSET       0x0800
#define SEG4_OFFSET       0x0900
#define SEG5_OFFSET       0x0A00
#define SEG6_OFFSET       0x0B00

unsigned char seg_font[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F };
unsigned short offset[6] = { SEG1_OFFSET, SEG2_OFFSET, SEG3_OFFSET, SEG4_OFFSET, SEG5_OFFSET, SEG6_OFFSET };

int main(void)
{
	int fd;
	int i, num, seg;
	unsigned char *CS3_base;
	unsigned char *seg_port;

	fd = open("/dev/mem", O_RDWR);
	if (fd < 0)
	{
		perror("/dev/mem FAIL! \n");
		exit(1);
	}

	for (num = 0; num < 10; num++)
	{
		printf("Segement Display = %d \n", num);
		CS3_base = (unsigned char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CS3_BASE);
		if ((int)CS3_base == -1)
		{
			perror("mmap FAIL! \n");
			return -1;
		}

		for (seg = 0; seg < 6; seg++)
		{
			seg_port = (unsigned char *)(CS3_base + offset[seg]);
			*seg_port = seg_font[num];
		}

		munmap((unsigned char *)CS3_base, 1024);

		usleep(1000000);
	}

	close(fd);
	return 0;
}
