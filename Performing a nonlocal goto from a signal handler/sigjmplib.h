#ifndef SIGJMPLIB_H
#define SIGJMPLIB_H
#include    "general.h"

/*
 * Set to 1 once "env" buffer has been
 * initialized by [sig]setjmp()
 */
extern volatile sig_atomic_t canJump;

#ifdef USE_SIGSETJMP
sigjmp_buf senv;
#else
jmp_buf env;
#endif

void handler (int sig);

#endif
