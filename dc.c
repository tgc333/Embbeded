#include <unistd.h>
#include <fcntl.h>

main()
{
	int fd;
	char buffer[1024] = "[user] this is user application";
	fd = open("/dev/hello", O_RDWR);
	write(fd, buffer, strlen(buffer) + 1);
	read(fd, buffer, 1024);
	printf("%s\n", buffer);
	close(fd);
}
