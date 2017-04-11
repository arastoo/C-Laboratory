#include	<stdio.h>
#include	<stdlib.h>
#include	<termios.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<string.h>


#define		MAX_BUF		200


ssize_t readAll(int, void *, size_t);
void errExit(char *, int);


int 
main(void)
{
	char	buf[MAX_BUF + 1];
	int		fd_stdin;
	ssize_t bytes_read;

	memset(buf, '\0', MAX_BUF + 1);

	errno = 0;
	fd_stdin = fileno(stdin);
	if(-1 == fd_stdin) errExit("fileno function failed", errno);

	bytes_read = readAll(fd_stdin, buf, MAX_BUF);

	printf("\n%ld bytes read.\n", bytes_read);
	printf("%s\n", buf);

	exit(EXIT_SUCCESS);
}/* main(void) */


ssize_t 
readAll(int fd, void * data, size_t count)
{
	ssize_t		bytes_read;
	size_t		total = 0;
	char *		data_ptr = data;

	while(count)
	{
		errno = 0;
		bytes_read = read(fd, data_ptr, count);
		if(-1 == bytes_read) errExit("read system call failed", errno);

		data_ptr += bytes_read;
		count -= bytes_read;
		total += bytes_read;
	}/* while(count)*/

	errno = 0;
	if(-1 == tcflush(fd, TCIOFLUSH)) errExit(NULL, errno);

	return total;
}/* readAll(int fd, void * data, size_t count) */


void 
errExit(char * msg, int err_number)
{
	if(NULL != msg) fprintf(stderr, "Error Message: %s.\n", msg);
	fprintf(stderr, "Error: %s.\n", strerror(err_number));
	exit(EXIT_FAILURE);
}/* errExit(char * msg, int err_number) */
