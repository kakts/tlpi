/**
 * @file signal.c
 * 
 * 22-1 signal()の実装
 */

#if defined(__sun) || defined(__sgi)
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int sig, sighandler_t handler)
{
    struct sigaction newDisp, prevDisp;

    newDisp.sa_handler = handler;
    sigemptyset(&newDisp.sa_mask);

#ifdef OLD_SIGNAL
    newDisp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
    newDisp.sa_flags = SA_RESTART;
#endif

    if (sigaction(sig, &newDisp, &prevDisp) == -1) {
        return SIG_ERR;
    }
    return prevDisp.sa_handler;
}
#else

extern int signalDummyVar;
#endif
