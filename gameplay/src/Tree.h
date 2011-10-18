/*
 * Tree.h
 */

#ifndef TREE_H_
#define TREE_H_

namespace gameplay
{

/**
 * Defines a class that provides a tree hierarchy, which supporting
 * a doubly linked list of siblings and a list of children.
 */
template <class T>
class Tree
{
public:

    Tree();

    /**
     * Destructor.
     */
    virtual ~Tree();

    virtual void addChild(T* child);

    virtual void removeChild(T* child);

    virtual void removeAllChildren();

    T* getFirstChild() const;

    T* getNextSibling() const;

    T* getPreviousSibling() const;

    T* getParent() const;

    unsigned int getChildCount() const;

protected:

    /**
     * Removes this item from its list.
     */
    void remove();

    virtual void childAdded(T* child);

    virtual void childRemoved(T* child);

    virtual void parentChanged(T* oldParent);

    T* _firstChild;
    T* _nextSibling;
    T* _prevSibling;
    T* _parent;
    unsigned int _childCount;
};

template <class T>
Tree<T>::Tree()
    : _firstChild(NULL), _nextSibling(NULL), _prevSibling(NULL), _parent(NULL), _childCount(0)
{
}

template <class T>
Tree<T>::~Tree()
{
    removeAllChildren();
}

template <class T>
void Tree<T>::addChild(T* child)
{
    assert(child);

    // If the item belongs to another hierarchy, remove it first.
    T* parent = child->_parent;
    if (parent)
    {
        parent->removeChild(child);
    }

    // Order is irrelevant, so add to the beginning of the list.
    if (_firstChild)
    {
        _firstChild->_prevSibling = child;
        child->_nextSibling = _firstChild;
        _firstChild = child;
    }
    else
    {
        _firstChild = child;
    }

    child->_parent = static_cast<T*>(this);

    ++_childCount;

    // Fire events.
    static_cast<Tree<T>*>(child)->parentChanged(parent);
    childAdded(child);
}

template <class T>
void Tree<T>::removeChild(T* child)
{
    if (child == NULL || child->_parent != this)
    {
        // The child is not in our hierarchy.
        return;
    }

    // Call remove on the child.
    static_cast<Tree<T>*>(child)->remove();
}

template <class T>
void Tree<T>::removeAllChildren()
{
    while (_firstChild)
    {
        removeChild(_firstChild);
    }
}

template <class T>
void Tree<T>::remove()
{
    // Re-link our neighbours.
    if (_prevSibling)
    {
        _prevSibling->_nextSibling = _nextSibling;
    }
    if (_nextSibling)
    {
        _nextSibling->_prevSibling = _prevSibling;
    }

    // Update our parent.
    T* parent = _parent;
    if (parent)
    {
        if (this == parent->_firstChild)
        {
            parent->_firstChild = _nextSibling;
        }

        --parent->_childCount;
    }

    _nextSibling = NULL;
    _prevSibling = NULL;
    _parent = NULL;

    // Fire events.
    if (parent)
    {
        parentChanged(parent);
        static_cast<Tree<T>*>(parent)->childRemoved(static_cast<T*>(this));
    }
}

template <class T>
T* Tree<T>::getFirstChild() const
{
    return _firstChild;
}

template <class T>
T* Tree<T>::getNextSibling() const
{
    return _nextSibling;
}

template <class T>
T* Tree<T>::getPreviousSibling() const
{
    return _prevSibling;
}

template <class T>
T* Tree<T>::getParent() const
{
    return _parent;
}

template <class T>
unsigned int Tree<T>::getChildCount() const
{
    return _childCount;
}

template <class T>
void Tree<T>::childAdded(T* child)
{
}

template <class T>
void Tree<T>::childRemoved(T* child)
{
}

template <class T>
void Tree<T>::parentChanged(T* oldParent)
{
}

}

#endif
