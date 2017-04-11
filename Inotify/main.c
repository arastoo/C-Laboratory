#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<sys/inotify.h>
#include	<limits.h>
#include	<string.h>


#define		BUF_LEN		(10 * (sizeof(struct inotify_event) + NAME_MAX + 1))


void displayInotifyEvent(struct inotify_event * i);
void errExit(char * msg);

int 
main(int argc, char * argv[])
{
	int		inotifyFd;
	int		wd;
	int		j;
	char	buf[BUF_LEN];
	char *	p;
	ssize_t numRead;
	struct	inotify_event	* event;

	if(argc < 2 || strcmp(argv[1], "--help") == 0) 
		errExit("Usage Error");

	inotifyFd = inotify_init();
	if(-1 == inotifyFd) 
		errExit("inotify_init");

	for(j = 1; j < argc; j++)
	{
		wd = inotify_add_watch(inotifyFd, argv[j],IN_ALL_EVENTS);
		if(-1 == wd) 
			errExit("inotify_add_watch");

		printf("Watching %s using wd %d\n", argv[j], wd);
	
	}/* for(j = 1; j < argc; j++) */

	for(;;)
	{
		numRead = read(inotifyFd, buf, BUF_LEN);
		if(0 == numRead)
			errExit("read() from inorify fd returned 0!");

		if(-1 == numRead)
			errExit("read");

		printf("Read %ld bytes from inotify fd\n", (long) numRead);

		for(p = buf; p < buf + numRead;)
		{
			event = (struct inotify_event *) p;
			displayInotifyEvent(event);

			p += sizeof(struct inotify_event) + event->len;
		}/* for(p = buf; p < buf + numRead;) */
	}/* for(;;) */

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */

void 
displayInotifyEvent(struct inotify_event * i)
{
	printf("\twd = %2d; ", i->wd);

	if(i->cookie > 0) printf("cookie = %4d; ", i->cookie);

	printf("mask = ");
	if(i->mask & IN_ACCESS)			printf("IN_ACCESS");
	if(i->mask & IN_ATTRIB)			printf("IN_ATTRIB");
	if(i->mask & IN_CLOSE_NOWRITE)	printf("IN_CLOSE_NOWRITE");
	if(i->mask & IN_CLOSE_WRITE)	printf("IN_CLOSE_WRITE");
	if(i->mask & IN_CREATE)			printf("IN_CREATE");
	if(i->mask & IN_DELETE)			printf("IN_DELETE");
	if(i->mask & IN_IGNORED)		printf("IN_IGNORED");
	if(i->mask & IN_ISDIR)			printf("IN_ISDIR");
	if(i->mask & IN_MODIFY)			printf("IN_MODIFY");
	if(i->mask & IN_MOVE_SELF)		printf("IN_MOVED_SELF");
	if(i->mask & IN_MOVED_FROM)		printf("IN_MOVED_FROM");
	if(i->mask & IN_MOVED_TO)		printf("IN_MOVED_TO");
	if(i->mask & IN_OPEN)			printf("IN_OPEN");
	if(i->mask & IN_Q_OVERFLOW)		printf("IN_Q_OVERFLOW");
	if(i->mask & IN_UNMOUNT)		printf("IN_UNMOUNT");
	printf("\n");

	if(i->len > 0) printf("\t\tname = %s\n", i->name);

}/* displayInotifyEvent(struct inotify_event * i) */

void 
errExit(char * msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}/* errExit(char * msg) */
