#include    "general.h"
#include    "tlib.h"

void 
write_to_thread_log (const char * message)
{
    FILE *  thread_log = (FILE *) pthread_getspecific (thread_log_key);

    fprintf (thread_log, "%s\n", message);
}

void 
close_thread_log (void * thread_log)
{
    fclose ((FILE *) thread_log);
}

void * 
thread_function (void * args)
{
    char    thread_log_filename[20];
    FILE *  thread_log;

    memset (thread_log_filename, 0, 20);

    sprintf (thread_log_filename, "thread_%d.log", (int) pthread_self ());
    thread_log = fopen (thread_log_filename, "w");
    pthread_setspecific (thread_log_key, thread_log);

    write_to_thread_log ("Thread Starting.");

    return NULL;
}


