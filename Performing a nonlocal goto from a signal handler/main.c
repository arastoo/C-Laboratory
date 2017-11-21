#include    "general.h"
#include    "sigjmplib.h"
#include    "siglib.h"
#include    "error.h"

volatile sig_atomic_t canJump = FALSE;

int 
main (int argc, char * argv[])
{
    struct sigaction sa;

    printSigMask(stdout, "Signal mask at startup:\n");

    sigemptyset (&sa.sa_mask);
    sa.sa_flags     = 0;
    sa.sa_handler   = handler;

    if (-1 == sigaction (SIGINT, &sa, NULL))
        errExit ("Sigaction");

    #ifdef USE_SIGSETJMP
        printf ("Calling sigsetjmp()\n");
        if (0 == sigsetjmp (senv, 1)) {
    #else
        printf ("Calling setjmp()\n");
        if (0 == setjmp (env)) {
    #endif
            canJump = TRUE;
        } else {
            printSigMask (stdout, "After jump from handler, signal mask is:\n");
        }

        for (;;)
            pause ();
}
