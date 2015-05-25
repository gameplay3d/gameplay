#ifndef REF_H_
#define REF_H_

namespace gameplay
{

/**
 * Defines the base class for game objects that require lifecycle management.
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

    /**
     * Returns the current reference count of this object.
     *
     * @return This object's reference count.
     */
    unsigned int getRefCount() const;

protected:

    /**
     * Constructor.
     */
    Ref();

    /**
     * Copy constructor.
     * 
     * @param copy The Ref object to copy.
     */
    Ref(const Ref& copy);

    /**
     * Destructor.
     */
    virtual ~Ref();

private:

    unsigned int _refCount;

    // Memory leak diagnostic data (only included when GP_USE_MEM_LEAK_DETECTION is defined)
#ifdef GP_USE_MEM_LEAK_DETECTION
    friend class Game;
    static void printLeaks();
    void* __record;
#endif
};

}

#endif
