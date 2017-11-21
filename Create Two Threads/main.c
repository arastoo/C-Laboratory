#include    "general.h"

typedef struct {
    char    message[100];
    int     count;
} thread_arg_t;

void * thread_handler (void *);

int
main (void) 
{
    int             thread_0_retval;
    int             thread_1_retval;
    int             thread_0_exit_status;
    int             thread_1_exit_status;
    pthread_t       thread_0_id;
    pthread_t       thread_1_id;
    thread_arg_t    arg0;
    thread_arg_t    arg1;

    memset (&arg0, 0, sizeof (thread_arg_t));
    memset (&arg1, 0, sizeof (thread_arg_t));

    arg0.count  = 50;
    strncpy (arg0.message, "This is First thread", 20);

    arg1.count  = 100;
    strncpy (arg1.message, "This is Seconed thread", 22);

    thread_0_retval = pthread_create (&thread_0_id, NULL, &thread_handler, &arg0);
    if (0 != thread_0_retval)
        errExit ("Thread 0");

    thread_1_retval = pthread_create (&thread_1_id, NULL, &thread_handler, &arg1);
    if (0 != thread_1_retval)
        errExit ("Thread 1");
    
    if (0 != pthread_join (thread_0_id, (void *) &thread_0_exit_status))
        errExit ("Pthread_join for Thread 0");
    
    if (0 != pthread_join (thread_1_id, (void *) &thread_1_exit_status))
        errExit ("Pthread_join for Thread 1");

    fprintf(stdout, "Thread 0 Ended with %d status code.\n", thread_0_exit_status);
    fprintf(stdout, "Thread 1 Ended with %d status code.\n", thread_1_exit_status);
    
    exit (EXIT_SUCCESS);
}

void * 
thread_handler (void * parameter) 
{
    int         i;
    
    thread_arg_t    * arg = (thread_arg_t *) parameter;

    for (i = 0; i < arg->count; i = i + 1)
        fprintf (stdout, "Thread ID: %d,\t%s\n",
                (int) pthread_self (), arg->message);

    pthread_exit ((void *) 0);
}
