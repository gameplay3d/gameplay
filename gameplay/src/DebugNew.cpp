#ifdef GAMEPLAY_MEM_LEAK_DETECTION

#include <new>
#include <exception>
#include <cstdio>
#include <cstdarg>

struct MemoryAllocationRecord
{
    unsigned long address;          // address returned to the caller after allocation
    unsigned int size;              // size of the allocation request
    const char* file;               // source file of allocation request
    int line;                       // source line of the allocation request
    MemoryAllocationRecord* next;
    MemoryAllocationRecord* prev;
};

MemoryAllocationRecord* __memoryAllocations = 0;
int __memoryAllocationCount = 0;

void* debugAlloc(size_t size, const char* file, int line);
void debugFree(void* p);

#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

void* operator new (size_t size, const char* file, int line)
{
    return debugAlloc(size, file, line);
}

void* operator new[] (size_t size, const char* file, int line)
{
    return operator new (size, file, line);
}

void* operator new (size_t size) throw(bad_alloc)
{
    return operator new (size, "", 0);
}

void* operator new[] (size_t size) throw(bad_alloc)
{
    return operator new (size, "", 0);
}

void* operator new (size_t size, const nothrow_t&) throw()
{
    return operator new (size, "", 0);
}

void* operator new[] (size_t size, const nothrow_t&) throw()
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

void* debugAlloc(size_t size, const char* file, int line)
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

    if (__memoryAllocations)
        __memoryAllocations->prev = rec;
    __memoryAllocations = rec;
    ++__memoryAllocationCount;

    return mem;
}

void debugFree(void* p)
{
    assert(p);

    // Backup passed in pointer to access memory allocation record
    void* mem = ((unsigned char*)p) - sizeof(MemoryAllocationRecord);

    MemoryAllocationRecord* rec = (MemoryAllocationRecord*)mem;

    // Sanity check: ensure that address in record matches passed in address
    if (rec->address != (unsigned long)p)
    {
        gameplay::printError("[memory] CORRUPTION: Attempting to free memory address with invalid memory allocation record.");
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

extern void printMemoryLeaks()
{
    // Dump general heap memory leaks
    if (__memoryAllocationCount == 0)
    {
        gameplay::printError("[memory] All HEAP allocations successfully cleaned up (no leaks detected).");
    }
    else
    {
        gameplay::printError("[memory] WARNING: %d HEAP allocations still active in memory.", __memoryAllocationCount);
        MemoryAllocationRecord* rec = __memoryAllocations;
        while (rec)
        {
            gameplay::printError("[memory] LEAK: HEAP allocation leak of size %d leak from line %d in file '%s'.", rec->size, rec->line, rec->file);
            rec = rec->next;
        }
    }
}

#endif
