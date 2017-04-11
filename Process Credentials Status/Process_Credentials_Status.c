#ifndef		_GNU_SOURCE
#define		_GNU_SOURCE
#endif


#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<errno.h>
#include	<string.h>


typedef enum {FALSE, TRUE} BOOLEAN;

int 
main(void)
{
	uid_t	r_uid;
	uid_t	e_uid;
	uid_t	s_uid;
	gid_t	r_gid;
	gid_t	e_gid;
	gid_t	s_gid;
	int		error_number;
	int		OF_DROP_PRIVILAGE = FALSE;
	int		OF_RETRIEVE_PRIVILAGE = FALSE;

	printf("\n----------------------------------------------------------------------\n\n");
	
	LOOP:
	
	errno = 0;
	if(-1 == getresuid(&r_uid, &e_uid, &s_uid))
	{
		error_number = errno;
		fprintf(stderr, "Error: %s\n", strerror(error_number));
		exit(EXIT_FAILURE);
	}/* if(-1 == getresuid(&r_uid, &e_uid, &s_uid)) */
	
	errno = 0;
	if(-1 == getresgid(&r_gid, &e_gid, &s_gid))
	{
		error_number = errno;
		fprintf(stderr, "Error: %s\n", strerror(error_number));
		exit(EXIT_FAILURE);
	}/* if(-1 == getresuid(&r_uid, &e_uid, &s_uid)) */

	printf("Real user ID:\t\t%d\t\tReal group ID:\t\t%d\n", r_uid, r_gid);
	printf("Effective user ID:\t%d\t\tEffective group ID:\t%d\n", e_uid, e_gid);
	printf("Saved user ID:\t\t%d\t\tSaved group ID:\t\t%d\n", s_uid, s_gid);
	printf("\n----------------------------------------------------------------------\n\n");

	if(0 != r_uid && 0 == e_uid && 0 == s_uid && !OF_DROP_PRIVILAGE)
	{
		setresgid(-1, r_gid, -1);
		setresuid(-1, r_uid, -1);
		OF_DROP_PRIVILAGE = TRUE;
		printf("Process temporarily drop the privilege.\n");
		goto LOOP;
	}/* if(0 != r_uid && 0 == e_uid && 0 == s_uid && !OF_DROP_PRIVILAGE) */

	if(0 != r_uid && 0 != e_uid && 0 == s_uid && !OF_RETRIEVE_PRIVILAGE)
	{
		setresgid(-1, s_gid, -1);
		setresuid(-1, s_uid, -1);
		OF_DROP_PRIVILAGE = TRUE;
		printf("Process regain the privilege.\n");
		goto LOOP;
	}/* if(0 != r_uid && 0 != e_uid && 0 == s_uid && !OF_RETRIEVE_PRIVILAGE) */

	exit(EXIT_SUCCESS);
}/* main(void) */
