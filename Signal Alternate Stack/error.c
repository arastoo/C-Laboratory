#include	<stdarg.h>
#include	"general.h"
#include	"ename.c.inc"


#ifdef	__GNUC__
	__attribute__ ((__noreturn__))
#endif
static void 
terminate(Boolean use_exit3)
{
	char	*	s;

	s = getenv("EF_DUMPCORE");

	if(NULL != s && '\0' != *s)
	{
		abort();
	}/* if(NULL != s && '\0' != *s) */
	else if(use_exit3)
	{
		exit(EXIT_FAILURE);
	}/* else if(use_exit3) */
	else
	{
		_exit(EXIT_FAILURE);
	}/* else */

}/* terminate(Boolean useExit3) */


static void
outputError(Boolean use_error, int error, Boolean flush_stdout,
		const char * format, va_list ap)
{
#define		BUF_SIZE	500

	char	buf[BUF_SIZE];
	char	user_msg[BUF_SIZE];
	char	err_txt[BUF_SIZE];
	
	vsnprintf(user_msg, BUF_SIZE, format, ap);

	if(use_error)
	{
		snprintf(err_txt, BUF_SIZE, " [%s %s] ",
				(error > 0 && error <= MAX_ENAME) ? 
				ename[error] : "?UNKNOWN?",
				strerror(error));
	}/* if(use_error) */
	else
	{
		snprintf(err_txt, BUF_SIZE, ":");
	}/* else */

	snprintf(buf, BUF_SIZE, "ERROR%s %s\n", err_txt, user_msg);

	if(flush_stdout)
	{
		fflush(stdout);
	}/* if(flush_stdout) */

	fputs(buf, stderr);
	fflush(stderr);

}/* outputError(Boolean use_error, int error, Boolean flush_stdout, */

void
usageError(const char * format, ...)
{
	va_list		arg_list;

	fflush(stdout);

	fprintf(stderr, "Usage: ");
	va_start(arg_list, format);
	vfprintf(stderr, format, arg_list);
	va_end(arg_list);

	fflush(stderr);

	exit(EXIT_FAILURE);
}/* usageError(const char * format, ...) */

void
errExit(const char * format, ...)
{
	va_list		arg_list;

	va_start(arg_list, format);
	outputError(TRUE, errno, TRUE, format, arg_list);
	va_end(arg_list);

	terminate(TRUE);
}/* errExit(const char * format, ...) */
