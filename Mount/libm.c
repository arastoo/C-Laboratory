#ifndef		LIBM
#define		LIBM

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>
#include	<blkid/blkid.h>
#include	<sys/mount.h>

#include	"libue.h"
#include	"libm.h"


#define		MAX_BUFFER_SIZE		200
#define		FAIL				-1
#define		SUCCESS				0

typedef enum {
	BLOCK_DEVICE		= 1,
	CHARACTER_DEVICE	= 2,
	DIRECTORY			= 3,
	FIFO				= 4,
	SYMBOLIC_LINK		= 5,
	SOCKET				= 6,
	REGULAR_FILE		= 7
} device_type;

static int pathExist(const char * const);
static char * detectFSType(const char * const);


/*
 * This function checks path existence.
 * If the path exists, detect and return what type it is. 
 */
static int
pathExist(const char * const path)
{
	struct stat		buf;

	errno = 0;
	if(-1 == stat(path, &buf))
	{
		errExit(NULL, errno);
	}/* if(-1 == stat(path, &buf)) */

	if(S_ISREG(buf.st_mode)) return REGULAR_FILE;
	else if(S_ISDIR(buf.st_mode)) return DIRECTORY;
	else if(S_ISCHR(buf.st_mode)) return CHARACTER_DEVICE;
	else if(S_ISBLK(buf.st_mode)) return BLOCK_DEVICE;
	else if(S_ISFIFO(buf.st_mode)) return FIFO;
	else if(S_ISLNK(buf.st_mode)) return SYMBOLIC_LINK;
	else if(S_ISSOCK(buf.st_mode)) return SOCKET;
	else return FAIL;

}/* pathExist(const char * const path) */

/*
 * This function detect and return filesystem type.
 */
static char *
detectFSType(const char * const path)
{
	blkid_probe		probe	= NULL;
	char *			fs_type	= NULL;
	int				ret;
	static char		buf[MAX_BUFFER_SIZE];

	probe = blkid_new_probe_from_filename(path);
	if(!probe)
	{
		errExit("blkid could not do new probe.", 0);
	}/* if(!probe) */

	if(-1 == blkid_probe_enable_partitions(probe, 1))
	{
		errExit("blkid could not do probe on partitions.", 0);
	}/* if(-1 == blkid_probe_enable_partitions(probe, 1)) */

	ret = blkid_do_fullprobe(probe);
	if(1 == ret)
	{
		errExit("blkid can not detected.", 0);
	}/* if(1 == ret) */

	if(-1 == ret)
	{
		errExit("blkid can not do full probe.", 0);
	}/* if(-1 == ret) */

	if(-1 == blkid_probe_lookup_value(probe, "TYPE",
				(const char **) &fs_type, NULL))
	{
		errExit("Blkid can not look up value.", 0);
	}/* if(-1 == blkid_probe_lookup_value(probe, "TYPE", */

	strncpy(buf, fs_type, MAX_BUFFER_SIZE - 1);
	blkid_free_probe(probe);
	
	return buf;
}/* detectFSType(const char * const path) */

int 
mountFS(const char * const source, const char * const destination)
{
	unsigned long	mFlags	= MS_NOEXEC | MS_NOSUID | MS_RDONLY | MS_NODEV;
	char *			ptr		= NULL;
	char			fs_type[MAX_BUFFER_SIZE];

	if(BLOCK_DEVICE != pathExist(source))
	{
		usage();
		errExit("The source moust be block device.", 0);
	}/* if(BLOCK_DEVICE != pathExist(source)) */

	if(DIRECTORY != pathExist(destination))
	{
		usage();
		errExit("Mount pint must be a directory.", 0);
	}/* if(DIRECTORY != pathExist(destination)) */

	memset(fs_type, '\0', MAX_BUFFER_SIZE);
	ptr = detectFSType(source);
	strncpy(fs_type, ptr, MAX_BUFFER_SIZE - 1);

	errno = 0;
	if(-1 == mount(source, destination, fs_type, mFlags, NULL))
	{
		errExit(NULL, errno);
	}/* if(-1 == mount(source, destination, fs_type, mFlags, NULL)) */

	return SUCCESS;
}/* mountFS(const char * const source, const char * const destination) */
#endif
