#include <new>
#include <exception>
#include <cstdio>
#include <cstdarg>

#ifdef WIN32
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib,"dbghelp.lib")

#define MAX_STACK_FRAMES 16
bool __trackStackTrace = false;
#endif

struct MemoryAllocationRecord
{
    unsigned long address;          // address returned to the caller after allocation
    unsigned int size;              // size of the allocation request
    const char* file;               // source file of allocation request
    int line;                       // source line of the allocation request
    MemoryAllocationRecord* next;
    MemoryAllocationRecord* prev;
#ifdef WIN32
    bool trackStackTrace;
    unsigned int pc[MAX_STACK_FRAMES];
#endif
};

MemoryAllocationRecord* __memoryAllocations = 0;
int __memoryAllocationCount = 0;

void* debugAlloc(std::size_t size, const char* file, int line);
void debugFree(void* p);

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

void* operator new (std::size_t size, const char* file, int line)
{
    return debugAlloc(size, file, line);
}

void* operator new[] (std::size_t size, const char* file, int line)
{
    return operator new (size, file, line);
}

void* operator new (std::size_t size) throw(std::bad_alloc)
{
    return operator new (size, "", 0);
}

void* operator new[] (std::size_t size) throw(std::bad_alloc)
{
    return operator new (size, "", 0);
}

void* operator new (std::size_t size, const std::nothrow_t&) throw()
{
    return operator new (size, "", 0);
}

void* operator new[] (std::size_t size, const std::nothrow_t&) throw()
{
    return operator new (size, "", 0);
}

void operator delete (void* p) throw()
{
    debugFree(p);
}

void operator delete[] (void* p) throw()
{
    operator delete (p);
}

void operator delete (void* p, const char* file, int line) throw()
{
    operator delete (p);
}

void operator delete[] (void* p, const char* file, int line) throw()
{
    operator delete (p);
}

#ifdef _MSC_VER
#pragma warning( default : 4290 )
#endif

// Include Base.h (needed for logging macros) AFTER new operator impls
#include "Base.h"

void* debugAlloc(std::size_t size, const char* file, int line)
{
    // Allocate memory + size for a MemoryAlloctionRecord
    unsigned char* mem = (unsigned char*)malloc(size + sizeof(MemoryAllocationRecord));

    MemoryAllocationRecord* rec = (MemoryAllocationRecord*)mem;

    // Move memory pointer past record
    mem += sizeof(MemoryAllocationRecord);

    rec->address = (unsigned long)mem;
    rec->size = size;
    rec->file = file;
    rec->line = line;
    rec->next = __memoryAllocations;
    rec->prev = 0;

    // Capture the stack frame (up to MAX_STACK_FRAMES) if we 
    // are running on Windows and the user has enabled it.
#if defined(WIN32)
    rec->trackStackTrace = __trackStackTrace;
    if (rec->trackStackTrace)
    {
        static bool initialized = false;
        if (!initialized)
        {
            if (!SymInitialize(GetCurrentProcess(), NULL, true))
                printError("Stack trace tracking will not work.\n");
            initialized = true;
        }
    
        // Get the current context (state of EBP, EIP, ESP registers).
        static CONTEXT context;
        RtlCaptureContext(&context);
    
        static STACKFRAME64 stackFrame;
        memset(&stackFrame, 0, sizeof(STACKFRAME64));

        // Initialize the stack frame based on the machine architecture.
#ifdef _M_IX86
        static const DWORD machineType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset = context.Eip;
        stackFrame.AddrPC.Mode = AddrModeFlat;
        stackFrame.AddrFrame.Offset = context.Ebp;
        stackFrame.AddrFrame.Mode = AddrModeFlat;
        stackFrame.AddrStack.Offset = context.Esp;
        stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#error "Machine architecture not supported!"
#endif

        // Walk up the stack and store the program counters.
        memset(rec->pc, 0, sizeof(rec->pc));
        for (int i = 0; i < MAX_STACK_FRAMES; i++)
        {
            rec->pc[i] = stackFrame.AddrPC.Offset;
            if (!StackWalk64(machineType, GetCurrentProcess(), GetCurrentThread(), &stackFrame,
                &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
            {
                break;
            }
        }
    }
#endif

    if (__memoryAllocations)
        __memoryAllocations->prev = rec;
    __memoryAllocations = rec;
    ++__memoryAllocationCount;

    return mem;
}

void debugFree(void* p)
{
    if (p == 0)
        return;

    // Backup passed in pointer to access memory allocation record
    void* mem = ((unsigned char*)p) - sizeof(MemoryAllocationRecord);

    MemoryAllocationRecord* rec = (MemoryAllocationRecord*)mem;

    // Sanity check: ensure that address in record matches passed in address
    if (rec->address != (unsigned long)p)
    {
        printError("[memory] CORRUPTION: Attempting to free memory address with invalid memory allocation record.\n");
        return;
    }

    // Link this item out
    if (__memoryAllocations == rec)
        __memoryAllocations = rec->next;
    if (rec->prev)
        rec->prev->next = rec->next;
    if (rec->next)
        rec->next->prev = rec->prev;
    --__memoryAllocationCount;

    // Free the address from the original alloc location (before mem allocation record)
    free(mem);
}

#ifdef WIN32
void printStackTrace(MemoryAllocationRecord* rec)
{
    const unsigned int bufferSize = 512;

    // Resolve the program counter to the corresponding function names.
    unsigned int pc;
    for (int i = 0; i < MAX_STACK_FRAMES; i++)
    {
        // Check to see if we are at the end of the stack trace.
        pc = rec->pc[i];
        if (pc == 0)
            break;

        // Get the function name.
        unsigned char buffer[sizeof(IMAGEHLP_SYMBOL64) + bufferSize];
        IMAGEHLP_SYMBOL64* symbol = (IMAGEHLP_SYMBOL64*)buffer;
        DWORD64 displacement;
        memset(symbol, 0, sizeof(IMAGEHLP_SYMBOL64) + bufferSize);
        symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
        symbol->MaxNameLength = bufferSize;
        if (!SymGetSymFromAddr64(GetCurrentProcess(), pc, &displacement, symbol))
        {
            printError("[memory] STACK TRACE: <unknown location>\n");
        }
        else
        {
            symbol->Name[bufferSize - 1] = '\0';

            // Check if we need to go further up the stack.
            if (strncmp(symbol->Name, "operator new", 12) == 0)
            {
                // In operator new or new[], keep going...
            }
            else
            {
                // Get the file and line number.
                if (pc != 0)
                {
                    IMAGEHLP_LINE64 line;
                    DWORD displacement;
                    memset(&line, 0, sizeof(line));
                    line.SizeOfStruct = sizeof(line);
                    if (!SymGetLineFromAddr64(GetCurrentProcess(), pc, &displacement, &line))
                    {
                        printError("[memory] STACK TRACE: %s - <unknown file>:<unknown line number>\n", symbol->Name);
                    }
                    else
                    {
                        const char* file = strrchr(line.FileName, '\\');
                        if(!file) 
                            file = line.FileName;
                        else
                            file++;
                        
                        printError("[memory] STACK TRACE: %s - %s:%d\n", symbol->Name, file, line.LineNumber);
                    }
                }
            }
        }
    }
}
#endif

extern void printMemoryLeaks()
{
    // Dump general heap memory leaks
    if (__memoryAllocationCount == 0)
    {
        printError("[memory] All HEAP allocations successfully cleaned up (no leaks detected).\n");
    }
    else
    {
        printError("[memory] WARNING: %d HEAP allocations still active in memory.\n", __memoryAllocationCount);
        MemoryAllocationRecord* rec = __memoryAllocations;
        while (rec)
        {
#ifdef WIN32
            if (rec->trackStackTrace)
            {
                printError("[memory] LEAK: HEAP allocation leak at address %#x of size %d:\n", rec->address, rec->size);
                printStackTrace(rec);
            }
            else
                printError("[memory] LEAK: HEAP allocation leak at address %#x of size %d from line %d in file '%s'.\n", rec->address, rec->size, rec->line, rec->file);
#else
                printError("[memory] LEAK: HEAP allocation leak at address %#x of size %d from line %d in file '%s'.\n", rec->address, rec->size, rec->line, rec->file);
#endif
            rec = rec->next;
        }
    }
}

#if defined(WIN32)
void setTrackStackTrace(bool trackStackTrace)
{
    __trackStackTrace = trackStackTrace;
}

void toggleTrackStackTrace()
{
    __trackStackTrace = !__trackStackTrace;
}
#endif
