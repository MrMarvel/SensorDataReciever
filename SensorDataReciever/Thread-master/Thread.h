#ifndef __THREAD_H_INCLUDED__
#define __THREAD_H_INCLUDED__

#include <windows.h>


//Extremely simple thread class. Uses either Windows Threads (win32)
//or pthreads (unix).
class Thread
{
    public:
        Thread();
        int CreateNewThread(void * arg);

    protected:
        static void * EntryPoint(void* pthis);
        void * Arg() const {return Arg_;}
        void Arg(void* a){Arg_ = a;}

    public:
        virtual void Setup() {};
        virtual void Run(void*) {};

    private:
        void * Arg_;
        HANDLE thread; //Thread handle
        DWORD lpId;
};

#endif  // THREAD_H_INCLUDED
