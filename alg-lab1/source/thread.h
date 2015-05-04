#ifndef THREAD_H_
#define THREAD_H_

//----------------------------------------------------------
// INCLUDES
//----------------------------------------------------------

#include <Windows.h>

//----------------------------------------------------------
// TYPES
//----------------------------------------------------------

typedef HANDLE Thread;

//----------------------------------------------------------
// METHODS
//----------------------------------------------------------

Thread Thread_Create (int (*func)(), void* param);
void   Thread_Destroy(Thread thread);
void   Thread_WaitForExit(Thread thread);

#endif
