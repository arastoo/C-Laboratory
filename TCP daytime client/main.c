#include	<sys/socket.h>
#include	<netinet/in.h>		/* sockaddr_in */
#include	<arpa/inet.h>		/* htons */
#include	<limits.h>			
#include	"general.h"


#define		SA		struct sockaddr


int 
main(int argc, char * argv[])
{
	/*
	 * IPv4 AF_INET sockets:
	 * 
	 * struct sockaddr_in {
	 * short            sin_family;   // e.g. AF_INET, AF_INET6
	 * unsigned short   sin_port;     // e.g. htons(3490)
	 * struct in_addr   sin_addr;     // see struct in_addr, below
	 * char             sin_zero[8];  // zero this if you want to
	 * };	
	 *
	 * struct in_addr {
	 * unsigned long s_addr;          // load with inet_pton()
	 * };
	 *
	 */

	int		sock_fd;
	int		number;
	char	rec_line[LINE_MAX + 1];
	struct	sockaddr_in		server_address;

	if(2 != argc)
		usageError("%s [ip-address]\n", argv[0]);

	errno = 0;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
		errExit("socket");

	bzero(&server_address, sizeof(server_address));
	server_address.sin_family	= AF_INET;
	/* convert values between hotst and network byte order */
	server_address.sin_port		= htons(13);
	/* inet_pton function convert IPv4 and 6 address from text to binary form */
	if( 0 >= inet_pton(AF_INET, argv[1], &server_address.sin_addr))
		errExit("inet_pton");

	/* connect a socket  */
	if(-1 == connect(sock_fd, (SA *) &server_address, sizeof(server_address)))
		errExit("connect error");

	while(0 < (number = read(sock_fd, rec_line, LINE_MAX)))
	{
		rec_line[number] = 0;
		if(EOF == fputs(rec_line, stdout))
			errExit("fputs Error");
	}/* while(0 < (number = read(sock_fd, rec_line, LINE_MAX))) */
	if(-1 == number)
		errExit("read error");

	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */
