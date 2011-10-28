/*
 * Tree.h
 */

#ifndef TREE_H_
#define TREE_H_

namespace gameplay
{

/**
 * Defines a class that provides a tree hierarchy, which includes
 * a doubly linked list of siblings and a list of children.
 *
 * This class is intended to be extended rather than used as a stand-alone
 * data structure (this is reflected by the protected constructor). Classes
 * wishing to expose a tree-like structure may inherit this class. An example
 * is the Node class, which extends Tree<Node> to provide support for nesting of
 * nodes within nodes and tracking of parent/child relationships in a scene
 * graph.
 */
template <class T>
class Tree
{
public:

    /**
     * Adds a child to this item in the tree.
     *
     * @param child The child to add.
     */
    virtual void addChild(T* child);

    /**
     * Removes a child from this item in the tree.
     *
     * @param child The child to remove.
     */
    virtual void removeChild(T* child);

    /**
     * Removes all children under this item in the tree.
     */
    virtual void removeAllChildren();

    /**
     * Returns the first child for this item in the tree.
     *
     * @return The first child.
     */
    T* getFirstChild() const;

    /**
     * Returns the first sibling of this item in the tree.
     *
     * @return The first sibling.
     */
    T* getNextSibling() const;

    /**
     * Returns the previous sibling to this item in the tree.
     *
     * @return The previous sibling.
     */
    T* getPreviousSibling() const;

    /**
     * Returns the parent of this item in the tree.
     *
     * @return The parent.
     */
    T* getParent() const;

    /**
     * Returns the number of direct children of this item.
     *
     * @return The number of children.
     */
    unsigned int getChildCount() const;

protected:

    /**
     * Constructor.
     */
    Tree();

    /**
     * Destructor.
     */
    virtual ~Tree();

    /**
     * Removes this item from its list.
     */
    void remove();

    /**
     * Called when a child is added to this item in the tree.
     * 
     * @param child The child that was added.
     */
    virtual void childAdded(T* child);

    /**
     * Called when a child is removed from this item in the tree.
     *
     * @param child The child that was removed.
     */
    virtual void childRemoved(T* child);

    /**
     * Called when the parent of this item changes.
     *
     * @param oldParent The previous parent for this item.
     */
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
