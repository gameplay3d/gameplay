#ifndef THREAD_H_
#define THREAD_H_

namespace gameplay
{

#ifdef WIN32

    #include <Windows.h>

    typedef HANDLE THREAD_HANDLE;

    struct WindowsThreadData
    {
        int(*threadFunction)(void*);
        void* arg;
    };

    DWORD WINAPI WindowsThreadProc(LPVOID lpParam)
    {
        WindowsThreadData* data = (WindowsThreadData*)lpParam;
        int(*threadFunction)(void*) = data->threadFunction;
        void* arg = data->arg;
        delete data;
        data = NULL;
        return threadFunction(arg);
    }

    static bool createThread(THREAD_HANDLE* handle, int(*threadFunction)(void*), void* arg)
    {
        WindowsThreadData* data = new WindowsThreadData();
        data->threadFunction = threadFunction;
        data->arg = arg;
        *handle = CreateThread(NULL, 0, &WindowsThreadProc, data, 0, NULL);
        return (*handle != NULL);
    }

    static void waitForThreads(int count, THREAD_HANDLE* threads)
    {
        WaitForMultipleObjects(count, threads, TRUE, INFINITE);
    }

    static void closeThread(THREAD_HANDLE thread)
    {
        CloseHandle(thread);
    }

#else

    #include <pthread.h>

    typedef pthread_t THREAD_HANDLE;

    struct PThreadData
    {
        int(*threadFunction)(void*);
        void* arg;
    };

    void* PThreadProc(void* threadData)
    {
        PThreadData* data = (PThreadData*)threadData;
        int(*threadFunction)(void*) = data->threadFunction;
        void* arg = data->arg;
        delete data;
        data = NULL;
        int retVal = threadFunction(arg);
        pthread_exit((void*)retVal);
    }

    static bool createThread(THREAD_HANDLE* handle, int(*threadFunction)(void*), void* arg)
    {
        PThreadData* data = new PThreadData();
        data->threadFunction = threadFunction;
        data->arg = arg;
        return pthread_create(handle, NULL, &PThreadProc, data) == 0;
    }

    static void waitForThreads(int count, THREAD_HANDLE* threads)
    {
        // Call join on all threads to wait for them to terminate.
        // This also frees any resources allocated by the threads,
        // essentially cleaning them up.
        for (int i = 0; i < count; ++i)
        {
            pthread_join(threads[i], NULL);
        }
    }

    static void closeThread(THREAD_HANDLE thread)
    {
        // nothing to do... waitForThreads (which calls join) cleans up
    }

#endif

}

#endif
