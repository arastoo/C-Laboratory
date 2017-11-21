#ifndef     TLIB_H
#define     TLIB_H

pthread_key_t    thread_log_key;

void    close_thread_log (void * thread_log);
void *  thread_function (void * args);

#endif
