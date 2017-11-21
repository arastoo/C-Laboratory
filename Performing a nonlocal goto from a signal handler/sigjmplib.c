#include    "sigjmplib.h"
#include    "siglib.h"

void 
handler (int sig)
{
    /*
     * UNSAFE: This handler uses non-async-signal-safe functions
     */

    printf ("Received signal %d (%s), signal mask is:\n",
            sig, strsignal (sig));
    printSigMask (stdout, NULL);

    if (!canJump) {
        printf ("'env' buffer not yet set, doing a simple return\n");
        return;
    }

    #ifdef USE_SIGSETJMP
        siglongjmp (senv, 1);
    #else
        longjmp (env, 1);
    #endif
}
