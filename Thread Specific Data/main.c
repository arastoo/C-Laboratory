#include    "general.h"
#include    "tlib.h"

int 
main (void)
{
    int     i;
    pthread_t   threads[5];

    pthread_key_create (&thread_log_key, close_thread_log);

    for (i = 0; i < 5; ++i)
        pthread_create (&(threads[i]), NULL, thread_function, NULL);

    for (i = 0; i < 5; ++i)
        pthread_join (threads[i], NULL);

    exit (EXIT_SUCCESS);
}
