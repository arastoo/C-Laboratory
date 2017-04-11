#ifndef		_DEFAULT_SOURCE 
#define		_DEFAULT_SOURCE
#endif
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<errno.h>
#include	<time.h>
#include	<utime.h>
#include	"timestamps_lib.h"
#include	"error_lib.h"


void 
displayStatus(const char * path)
{
	struct	stat	buf;

	errno = 0;
	if(-1 == stat(path, &buf)) errExit(NULL, errno);

	printf("File type:\t\t\t");

	switch(buf.st_mode & S_IFMT)
	{
		case S_IFREG:	printf("regular file\n");			break;
		case S_IFDIR:	printf("directory\n");				break;
		case S_IFCHR:	printf("character device\n");		break;
		case S_IFBLK:	printf("block device\n");			break;
		case S_IFLNK:	printf("symbolic (soft) link\n");	break;
		case S_IFIFO:	printf("FIFO or pipe\n");			break;
		case S_IFSOCK:	printf("socket\n");					break;
		default:		printf("unknown file type?\n");		break;
	}/* switch(buf->st_mode & S_IFMT) */

	printf("Device containing i-node:\tmajor=%ld and minor=%ld\n",
			(long) major(buf.st_dev), (long) minor(buf.st_dev));

	printf("I-node number:\t\t\t%ld\n", (long) buf.st_ino);
	printf("Mode:\t\t\t\t%lo\n", (unsigned long) buf.st_mode);

	if(buf.st_mode & (S_ISUID | S_ISGID | S_ISVTX))
		printf("special bits set:\t\t%s%s%s\n",
				(buf.st_mode & S_ISUID) ? "set-UID "	: "",
				(buf.st_mode & S_ISGID) ? "set-GID "	: "",
				(buf.st_mode & S_ISVTX) ? "sticky "		: "");

	printf("Number of (hard) links:\t\t%ld\n", (long) buf.st_nlink);
	printf("Ownership:\t\t\tUID=%ld and GID=%ld\n",
			(long) buf.st_uid, (long) buf.st_gid);

	if(S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode))
		printf("Device number (st_rdev):\tmajor=%ld and minor=%ld\n",
				(long) major(buf.st_rdev), (long) minor(buf.st_rdev));

	printf("File size:\t\t\t%lld bytes\n", (long long) buf.st_size);
	printf("Optimal I/O block size:\t\t%ld bytes\n", (long) buf.st_blksize);
	printf("512 blocks allocated:\t\t%lld\n", (long long) buf.st_blocks);

	printf("Last file access:\t\t%s", ctime(&buf.st_atime));
	printf("Last file modification:\t\t%s", ctime(&buf.st_mtime));
	printf("Last status change:\t\t%s\n", ctime(&buf.st_ctime));

}/* displayStatus(const char * const path) */


void
changingTimestamps(const char * path)
{
	struct	stat		stat_buf;
	struct	utimbuf		utime_buf;
	time_t				current_time;
	
	errno=0;
	if(-1 == stat(path, &stat_buf)) errExit(NULL, errno);

	errno=0;
	if((time_t) -1 == time(&current_time)) errExit(NULL, errno);

	utime_buf.actime = current_time;
	utime_buf.modtime = current_time;

	if(-1 == utime(path, &utime_buf)) errExit(NULL, errno);

}/* changingTimestamps(const char * path) */
