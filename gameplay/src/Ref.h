/*
 * Ref.h
 */

#ifndef REF_H_
#define REF_H_

namespace gameplay
{

/**
 * Base class for game objects that require lifecycle management.
 *
 * This class provides reference counting support for game objects that
 * contain system resources or data that is normally long lived and
 * referenced from possibly several sources at the same time. The built-in
 * reference counting eliminates the need for programmers to manually
 * keep track of object ownership and having to worry about when to
 * safely delete such objects.
 */
class Ref
{
public:

    /**
     * Increments the reference count of this object.
     *
     * The release() method must be called when the caller relinquishes its
     * handle to this object in order to decrement the reference count.
     */
    void addRef();

    /**
     * Decrements the reference count of this object.
     *
     * When an object is initially created, its reference count is set to 1.
     * Calling addRef() will increment the reference and calling release()
     * will decrement the reference count. When an object reaches a
     * reference count of zero, the object is destroyed.
     */
    void release();

protected:

    /**
     * Constructor.
     */
    Ref();

    /**
     * Destructor.
     */
    virtual ~Ref();

private:

    unsigned int _refCount;
};

}

#endif
