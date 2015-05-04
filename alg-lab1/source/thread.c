//----------------------------------------------------------
// INCLUDES
//----------------------------------------------------------

#include "thread.h"

#include <Windows.h>

#include <stdlib.h>

//----------------------------------------------------------
// TYPES
//----------------------------------------------------------

typedef struct {
    int   (*func)(void*);
    void* param;
} ThreadFunc;

//----------------------------------------------------------
// FUNCTIONS
//----------------------------------------------------------

static DWORD ThreadProc(LPVOID lpdwThreadParam) {
    ThreadFunc* tf = lpdwThreadParam;

    int   (*thread_func)(void*) = tf->func;
    void*  thread_param         = tf->param;

    free(tf);

    return thread_func(thread_param);
}

Thread Thread_Create(int (*func)(void*), void* param) {
    DWORD       dwThreadId;
    ThreadFunc* tf = malloc(sizeof(ThreadFunc));

    tf->func  = func;
    tf->param = param;

    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, tf, 0,
                        &dwThreadId);
}

void Thread_Destroy(Thread thread) {
    TerminateThread(thread, 0);
}

void Thread_WaitForExit(Thread thread) {
    if (WaitForSingleObject(thread, 5000) == WAIT_TIMEOUT)
        Thread_Destroy(thread);
}
