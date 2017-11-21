#include    "general.h"

sig_atomic_t    child_exit_status;

void clean_up_child_process (int);

int 
main(void)
{
    pid_t       child_pid;
	sigset_t	prev_mask;
	sigset_t	int_mask;
	struct		sigaction	sa;

    memset (&sa, 0, sizeof (sa));

	sigemptyset(&int_mask);
	sigaddset(&int_mask, SIGCHLD);

    sa.sa_flags        = 0;
    sa.sa_handler   = &clean_up_child_process;
    
    if (-1 == sigaction (SIGCHLD, &sa, NULL))
        errExit ("Sigaction");

    printf ("The main Program process ID is %d.\n", (int) getpid ());
    
    child_pid = fork ();
    if (0 != child_pid) {

	    if(-1 == sigprocmask(SIG_BLOCK, &int_mask, &prev_mask))
            errExit("sigprocmask - SIG_BLOCK");

        /* Critical section */
        //sleep(60);
        printf ("This is the parent process, with id %d\n", (int) getpid ());
        printf ("The child's Process ID is %d\n", (int) child_pid);
        printf ("The child's Process terminated with %d exit status.\n", child_exit_status);


	    if(-1 == sigprocmask(SIG_SETMASK, &prev_mask, NULL))
            errExit("sigprocmask - SIG_SETMASK");

    } else {
        printf ("This is the child process, with id %d\n", (int) getpid ());
    }

    exit (EXIT_SUCCESS);
}

void
clean_up_child_process (int signal_number) 
{
    int status;

    wait (&status);
    child_exit_status = status;
}
