#include	<time.h>
#include	<limits.h>
#include	"general.h"
#include	"wrapsock.h"


int 
main(void)
{
	int		listenfd, connfd;
	char	buff[LINE_MAX];
	time_t	ticks;
	struct	sockaddr_in		servaddr;

	Socket(AF_INET, SOCK_STREAM, 0, &listenfd);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port	= htons(13);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	for(;;)
	{
		Accept(listenfd, (SA *) NULL, NULL, &connfd);
		
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff));

		Close(connfd);
	}/* for(;;) */
}/* main(void)*/
