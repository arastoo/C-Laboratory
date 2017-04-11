#define		_XOPEN_SOURCE

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<string.h>
#include	<errno.h>
#include	<limits.h>
#include	<ctype.h>


enum OPERATION_FLAGS
{
	TRUE		= 0b1101,
	FALSE		= 0b0000,
	TXT			= 0b1000,
	HEX			= 0b1001,
	TXT_TRUE	= 0b1010,
	HEX_TRUE	= 0b1011,
	TXT_AND_HEX = 0b1010 
};


void errExit(char *, int);
void printHelp(FILE *);
void bufferOperation(int, int, int);


int 
main(int argc, char * argv[])
{
	int		fd;
	int		option;
	int		flags;
	int		err_number;
	int		OP_SEEK;		/* Operation flag, seek to byte offset from the start of file. */
	int		OP_WRITE;		/* Operation flag, write the string at current file offset. */
	int		OP_READ_TXT;	/* Operation flag, read length byte, and display them in text form. */
	int		OP_READ_HEX;	/* Operation flag, read length byte, and display them in hex form. */
	size_t	string_length;
	size_t	rt_length;		/* Read length bytes and display them in text. */
	size_t	rh_length;		/* Read length bytes and display them in hexadecimal. */
	size_t	number_written;
	char	file_path[PATH_MAX];
	char *	write_buf;
	off_t	offset;
	mode_t	mode;

	const	char * const optstr = ":hs:r:R:w:f:";
	
	OP_SEEK = OP_WRITE = OP_READ_HEX = OP_READ_TXT = FALSE;
	flags	= O_RDWR | O_CREAT | O_EXCL;
	mode	= S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	if(2 > argc)
	{
		printHelp(stdout);
		errExit("Program needs arguments", EINVAL);
	}/* if(2 > argc) */

	while(-1 != (option = getopt(argc, argv, optstr)))
	{
		switch(option)
		{
			case 'h':
				printHelp(stdout);
				exit(EXIT_SUCCESS);

				break;
			case 'f':
				string_length = strlen(optarg);
				strncpy(file_path, optarg, string_length + 1);

				continue;
			case 's':
				errno = 0;
				offset = strtol(optarg, NULL, 10);
				err_number = errno;

				if(0 != err_number) errExit("strtol function failed.", err_number);
				else OP_SEEK = TRUE;

				continue;
			case 'r':
				errno = 0;
				rt_length = strtol(optarg, NULL, 10);
				err_number = errno;

				if(0 != err_number) errExit("strtol function failed.", err_number);
				else OP_READ_TXT = TXT_TRUE;

				continue;
			case 'R':
				errno = 0;
				rh_length = strtol(optarg, NULL, 10);
				err_number = errno;

				if(0 != err_number) errExit("strtol function failed.", err_number);
				else OP_READ_HEX = HEX_TRUE;

				continue;
			case 'w':
				string_length = strlen(optarg);

				errno = 0;
				write_buf = malloc(string_length + 1);
				err_number = errno;

				if(NULL == write_buf && 0 != err_number)
					errExit("malloc function failed.", err_number);
				else 
				{
					strncpy(write_buf, optarg, string_length);
					OP_WRITE = TRUE;
				}/* else */

				continue;
			case ':':
				printHelp(stdout);
				errExit("Option need an argument.", '\0');

				break;
			case '?':
				fprintf(stderr, "%s ", optopt);
				errExit("Unrecognized option.", '\0');

				break;
		}/* switch(option) */
	}/* while(-1 != (option = getopt(argc, argv, optstr))) */

	if(argc > optind) errExit("Unrecognized argument.", '\0');

	errno = 0;
	if(-1 == (fd = open(file_path, flags, mode)))
	{
		err_number = errno;
		if(EEXIST == err_number)
		{
			errno = 0;
			if(-1 == (fd = open(file_path, O_RDWR)))
			{
				err_number = errno;
				errExit("Open system call failed.", err_number);
			}/* if(-1 == (fd = open(file_path, O_RDWR))) */
		}/* if(EEXIST == err_number) */
		else errExit("Open system call failed.", err_number);
	}/* if(-1 == (fd = open(file_path, flags, mode))) */

	if(OP_SEEK)
	{
		errno = 0;
		if(-1 == lseek(fd, offset, SEEK_SET))
		{
			err_number = errno;
			errExit("lseek system call failed.", err_number);
		}/* if(-1 == lseek(fd, offset, SEEK_SET)) */

		printf("%ld: seek succeeded\n", (long) offset);
	}/* if(OP_SEEK) */
	
	switch(((OP_READ_TXT & OP_READ_HEX) == TXT_AND_HEX) ? TXT_AND_HEX :
			((OP_READ_TXT & TRUE) == TXT) ? TXT :
			((OP_READ_HEX & TRUE) == HEX) ? HEX : FALSE)
	{
		case TXT:
			bufferOperation(fd, rt_length, TXT);
			
			break;
		case HEX:
			bufferOperation(fd, rh_length, HEX);
			
			break;
		case TXT_AND_HEX:
			bufferOperation(fd, rt_length, TXT);
			bufferOperation(fd, rh_length, HEX);
			
			break;
	}/* switch(OP_READ_TXT & OP_READ_HEX) */

	if(OP_WRITE)
	{
		string_length = strlen(write_buf);
		errno = 0;
		number_written = write(fd, write_buf, string_length);
		err_number = errno;

		if(-1 == number_written) errExit("write system call failed.", err_number);
		else printf("%s: wrote %ld bytes\n", write_buf, (long) string_length);

		free(write_buf);
	}/* if(OP_WRITE) */

	exit(EXIT_SUCCESS);

}/* main(int argc, char * argv[]) */


void
bufferOperation(int fd, int length, int op)
{
	size_t	number_read;
	int		err_number;
	int		index;
	char *	buffer;

	errno = 0;
	buffer = malloc(length + 1);
	err_number = errno;

	if(NULL == buffer && 0 != err_number)
		errExit("malloc function failed.", err_number);

	errno = 0;
	if(-1 == (number_read = read(fd, buffer, length)))
	{
		err_number = errno;
		errExit("read system call failed.", err_number);
	}/* if(-1 == (number_read = read(fd, buffer, length))) */

	switch(op)
	{
		case TXT:
			for(index = 0; index < number_read; index++)
				printf("%c", isprint((unsigned char) buffer[index]) ? buffer[index] : '?');
			
			break;
		case HEX:
			for(index = 0; index < number_read; index++)
				printf("%02x", buffer[index]);
			break;
	}/* switch(op) */

	printf("\n");

	free(buffer);
}/* bufferOperation(int fd ,int length, int op) */


void 
printHelp(FILE * std)
{
	char * msg = "[Program Name] [File] [OPTION]\n"
				"DESCRIPTION\n"
				"\t-h\t\tPrint this help.\n\n"
				"\t-f\t\tSpecify the file.\n\n"
				"\t-s\t\tSeek to byte offset from the start of the file.\n\n"
				"\t-r\t\tRead length bytes from the file,\n"
				"\t\t\tstarting at the current file offset,\n"
				"\t\t\tand display them in text form.\n"
				"\t\t\tThis option must come before w option.\n\n"
				"\t-R\t\tRead length bytes from the file,\n"
				"\t\t\tstarting at the current file offset,\n"
				"\t\t\tand display them in hexadecimal.\n"
				"\t\t\tThis option must come before w option.\n\n"
				"\t-w\t\tWrite the string of characters specified in str\n"
				"\t\t\tat thecurrent file offset.\n\n";

	fprintf(std, "%s", msg);
}/* printHelp(FILE std) */


void 
errExit(char * msg, int err_number)
{
	fprintf(stderr, "Error:%s, description: %s\n", msg, strerror(err_number));

	exit(EXIT_FAILURE);
}/* errExit(char * msg, int exit_code) */
