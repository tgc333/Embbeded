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
#define LED_OFFSET 0x0C00

int main(void)
{
	int fd;
	unsigned char *CS3_base;
	unsigned char *led;

	fd = open("/dev/mem", O_RDWR);
	if (fd < 0)
	{
		perror("/dev/mem FAIL! \n");
		exit(1);
	}

	CS3_base = (unsigned char *)mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CS3_BASE);
	if ((int)CS3_base == -1)
	{
		perror("mmap FAIL! \n");
		return -1;
	}
	led = CS3_base + LED_OFFSET;

	*led = 0x55;    // 2Áø¼ö : 0101 0101

	munmap((unsigned char *)CS3_base, 1024);

	close(fd);
	return 0;
}
