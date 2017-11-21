#include    "general.h"

void *  allocate_buffer (size_t);
void    deallocate_buffer (void *);
void *  do_some_work (void *);

int
main (void)
{
    int             ret;
    pthread_t       thread_id_0;

    ret = pthread_create (&thread_id_0, NULL, &do_some_work, NULL);
    if (0 != ret)
        errExit("Pthread Failed");

    sleep(10);
    pthread_cancel (thread_id_0);
    pthread_join (thread_id_0, NULL);

    exit (EXIT_SUCCESS);
}

void *
allocate_buffer (size_t size)
{
    printf ("Allocate Memory\n");
    return malloc (size);
}

void 
deallocate_buffer (void * buffer)
{
    printf ("Deallocate Memory\n");

    free (buffer);
}

void * 
do_some_work (void * arg)
{
    printf ("In Thread Function\n");
    
    void *  tmp_buffer = allocate_buffer (1024);
    pthread_cleanup_push (deallocate_buffer, tmp_buffer);

    sleep (60); 
    /*
     * Do some work here that might call pthread_exit or
     * might be cancelled ...
     */

    pthread_cleanup_pop (1);

    return NULL;
}
