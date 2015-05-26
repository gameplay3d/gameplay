#include "Base.h"
#include "Ref.h"
#include "Game.h"

namespace gameplay
{

#ifdef GP_USE_MEM_LEAK_DETECTION
void* trackRef(Ref* ref);
void untrackRef(Ref* ref, void* record);
#endif

Ref::Ref() :
    _refCount(1)
{
#ifdef GP_USE_MEM_LEAK_DETECTION
    __record = trackRef(this);
#endif
}

Ref::Ref(const Ref& copy) :
    _refCount(1)
{
#ifdef GP_USE_MEM_LEAK_DETECTION
    __record = trackRef(this);
#endif
}

Ref::~Ref()
{
}

void Ref::addRef()
{
    GP_ASSERT(_refCount > 0 && _refCount < 1000000);
    ++_refCount;
}

void Ref::release()
{
    GP_ASSERT(_refCount > 0 && _refCount < 1000000);
    if ((--_refCount) <= 0)
    {
#ifdef GP_USE_MEM_LEAK_DETECTION
        untrackRef(this, __record);
#endif
        delete this;
    }
}

unsigned int Ref::getRefCount() const
{
    return _refCount;
}

#ifdef GP_USE_MEM_LEAK_DETECTION

struct RefAllocationRecord
{
    Ref* ref;
    RefAllocationRecord* next;
    RefAllocationRecord* prev;
};

RefAllocationRecord* __refAllocations = 0;
int __refAllocationCount = 0;

void Ref::printLeaks()
{
    // Dump Ref object memory leaks
    if (__refAllocationCount == 0)
    {
        print("[memory] All Ref objects successfully cleaned up (no leaks detected).\n");
    }
    else
    {
        print("[memory] WARNING: %d Ref objects still active in memory.\n", __refAllocationCount);
        for (RefAllocationRecord* rec = __refAllocations; rec != NULL; rec = rec->next)
        {
            Ref* ref = rec->ref;
            GP_ASSERT(ref);
            const char* type = typeid(*ref).name();
            print("[memory] LEAK: Ref object '%s' still active with reference count %d.\n", (type ? type : ""), ref->getRefCount());
        }
    }
}

void* trackRef(Ref* ref)
{
    GP_ASSERT(ref);

    // Create memory allocation record.
    RefAllocationRecord* rec = (RefAllocationRecord*)malloc(sizeof(RefAllocationRecord));
    rec->ref = ref;
    rec->next = __refAllocations;
    rec->prev = 0;

    if (__refAllocations)
        __refAllocations->prev = rec;
    __refAllocations = rec;
    ++__refAllocationCount;

    return rec;
}

void untrackRef(Ref* ref, void* record)
{
    if (!record)
    {
        print("[memory] ERROR: Attempting to free null ref tracking record.\n");
        return;
    }

    RefAllocationRecord* rec = (RefAllocationRecord*)record;
    if (rec->ref != ref)
    {
        print("[memory] CORRUPTION: Attempting to free Ref with invalid ref tracking record.\n");
        return;
    }

    // Link this item out.
    if (__refAllocations == rec)
        __refAllocations = rec->next;
    if (rec->prev)
        rec->prev->next = rec->next;
    if (rec->next)
        rec->next->prev = rec->prev;
    free((void*)rec);
    --__refAllocationCount;
}

#endif

}
