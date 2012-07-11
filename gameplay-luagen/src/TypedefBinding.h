#ifndef TYPEDEFBINDING_H_
#define TYPEDEFBINDING_H_

/**
 * Represents a typedef.
 * 
 * Note: the name of the typedef is stored as the key in the 
 * map containing all typedefs. The refId member is set if
 * the typedef is a class, struct, or enumerated type.
 */
struct TypedefBinding
{
    string refId;
};

#endif
