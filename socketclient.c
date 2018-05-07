#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>



#include <fcntl.h>



#define BUFSIZE   1000



int main(void)

{

	int sockfd;

	struct sockaddr_in my_addr;

	struct sockaddr_in their_addr;

	int addr_len, numbytes;

	int myport = 5000;



	char buf[BUFSIZE];



	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)

	{

		perror("socket error!");

		exit(1);

	}



	my_addr.sin_family = AF_INET;

	my_addr.sin_port = htons(myport);

	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	memset(&(my_addr.sin_zero), '\0', 8);



	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)

	{

		perror("bind error!");

		exit(1);

	}



	addr_len = sizeof(struct sockaddr);



	if ((numbytes = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1)

	{

		perror("recvfrom error!");

		exit(1);

	}



	//buf[numbytes] = '\0';

	printf("%18s : %s\n", "client IP address", inet_ntoa(their_addr.sin_addr));

	printf("%18s : %d \n", "packet length", numbytes);

	printf("%18s : \"%s\"\n", "message", buf);



	close(sockfd);



	return 0;

}
