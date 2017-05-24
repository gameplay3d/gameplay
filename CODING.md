# C++ Coding Guidelines

This covers the basic coding guidline for all c++ code that is submitted to the project. 

## Files
Files should be named according to their class names:

Ex. 
class **Terrain** = **Terrain**.h + **Terrain**.cpp

## Naming
| Classes, Structs, Enums and Typedefs | Camel Case |
| --- | --- |
| Functions | camelCase |
| Private + Protected member variables | _camelCase |
| Public member variables | camelCase |
| Local variables | camelCase |
| Constants | ALL_CAPS |
| Enum values | TYPE_ALL_CAPS |


## Class Layout
```cpp
#pragma once

#include "Vector3.h";

namespace gameplay
{

class Foo
{
public:

    Foo();
    
    ~Foo();
    
    Vector3 getSimple() const;
    
    void setSimple(const Vector3& simple);

private:
    Vector3 _simple;
};

}
```

## Classes
- Each access modifier appears no more than once in a class, in the order:
**public**, **protected**, **private**.
- All **public** member variables live at start of the class.
- All **private** member variables live at the end of the class.
- Use **protected** member variables judiciously.
- Constructors and destructors are the first methods in a class after **public** member variables.
- The method implementations in cpp should appears in the order which they are declared in the class.
- Avoid **inline** implementations unless absolutely needed and are trivial and short.

## Enums
- Should be declared in classes directly after constructor and destructors.
- Keep the naming simple and short-and-sweet.
- Enum values should be prefixed with the **enum** type name:
```cpp
public:
    Camera();
    
    ~Camera();

    enum Mode
    {
        MODE_PERSPECTIVE,
        MODE_ORTHOGRAPHIC
    };

    Camera::Mode getMode() const;
```

## Smart Pointers vs Pointers
- Use **std::unique_pt<Foo>** whenever possible especially for public api pointers.
- Any code still using normal pointers should be slowly be migrated to **std::unique_ptr**.
- Any **delete** function call appearing in the code is a red flag and needs a good reason.
- Use **std::shared_ptr<Foo>** only when sharing is required.
- Use **std::vector** if you need an array and use **std::vector::data()** to get a raw pointer to the beginning of a vector.

## Auto
- Use **auto** sparingly and when needed to abstract and complex type.
- Great for use on **std::** stl containers and smart pointers.
- Be careful about accidental _copy-by-value_ when you meant _copy-by-reference_.
- Understand the difference between **auto** and **auto&**.

## Lambdas
- Lambdas are acceptable where they make sense.
- Focus use around anonymity.
- Avoid over use, but especially for std algorithms Ex. **std::sort**, etc) they are fine.

## Range-based loops
- They're great, use them.
- They don't have to be combined with **auto**.
- They are often more readable to not use auto for simple types:
```cpp
for (int dev : devices)  // More obvious that the iterator is a device index.
```
**vs.**
```cpp
for (const auto& dev : devices)  // Suggests dev might be of type Device.
```

## Friend
- Avoid using friend unless needed to really restrict access to internal classes.
- It easily leads to _difficult-to-untangle_ interdependencies that are hard to maintain.
- We will continue to remove where possible in existing code.

## size_t
- Use **size_t** for all size counts and accessing **std::size()** results.

## int
- We use **int** for all dimensions and lengths.

## unsigned int
- Avoid using **unsigned int** unless needed for bitwise flags, masks, etc.

## float
- The **float** type is the primary precision for all data in the 2d and 3d world.
- Used for low precision time intervals.
- Use **double** is _only_ used when absolutely necessary for higher precision.

## nullptr
- Use **nullptr** instead of _0_ or _NULL_.

## Errors and Logging
- Use **GP_ERROR** macro for all errors that will stop the program in release and debug modes.
```cpp
GP_ERROR("Invalid json base64 string for propertyName: %s", propertyName);
```
- Use **GP_INFO** for one time logging events such as initialization or unexpected singularities.
- Avoid excessive logging which can impact game engine performance, such as logging every frame.

## GP_ Macros and Global Constants
- Use existing **GP_XXX** for various compile time change functionality.
- Use **GP_XXX** as a prefix for all gameplay scoped macros and global constants.

# Assertions
- Use **GP_ASSERT** for quick danger checks in start of impl that is checked in debug mode.
```cpp
size_t SerializerJson::readFloatArray(const char* propertyName, float** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Serializer::TYPE_READER);
```

## Public member variable access
- Use **this->** when accessing public variables.
```cpp
void BoundingSphere::set(const Vector3& center, float radius)
{
    this->center =center;
    this->radius = radius;
    ...
```

## Public member variable initialization
- Initialize public member variables directly in .h file for awareness.
```cpp
class Rectangle
{
public:
    float x = 0.0f;
```

## Protected + private member variable initialization
- Initialize all **protected** and **private** member variables in constructor in cpp to obscure.
- Initialize all **protected** and **private** member variables in the order they are declared.
```cpp
Game::Game() :
        _config(nullptr),
        _state(Game::STATE_UNINITIALIZED),
        _width(GP_GRAPHICS_WIDTH),
         ...
```

## Commenting Headers
- Avoid spelling and grammatical errors.
- Header comments use _doxygen_ format. We are not too sticky on _doxygen_ formatting policy.
- All public functions and variables must be documented.
- The level of detail for the comment is based on the complexity for the api.
- Most important is that comments are simple and have clarity on how to use the api.
- **@brief** is dropped and automatic assumed on first line of code. Easier to read too.
- **@details** is dropped and automatic assumed proceeding the brief line.
- **@param** and **@return** are followed with a space after summary brief or details.

```cpp
    /**
     * Tests whether this bounding box intersects the specified bounding box.
     *
     * You would add any specific details that may be needed here. This is
     * only necessary if there is complexity to the user of the function.
     *
     * @param box The bounding box to test intersection with.
     * @return true if the specified bounding box intersects this bounding box;
     *     false otherwise.
     */
    bool intersects(const BoundingBox& box) const;
```
- Overridden functions can simply refer to the base class comments
```cpp
    /**
     * @see Serializable::onSerialize
     */
    void onSerialize(Serializer* serializer);
```

## Commenting Implementation
- Clean simple code is the best form of commenting.
- Do not add comments above function definitions in .cpp they are already in header.
- Used to comment necessary non-obvious implementation details not the api.
- Only use // line comments on the line above the code you plan to comment.
- Avoid /* */  block comments. Preventing others from easily doing their own block comments when testing, debugging, etc.
- Avoid explicitly referring to identifiers in comments, since that's an easy way to make your comment outdated when an identifier is renamed.

## Formatting
- You should set your ide or editor to follow the formatting guidelines.
- Keep all code less than **120** characters per line.

## Includes
- **#pragma** once is first line of the .h class header.
- **#include "Base.h"** is the first line of the .cpp class implementation.
- Include the corresponding class header to the second line just after including "Base.h".
- All the other headers, sorted by **local** to **distant** directories. (Ex. std headers come last)

## Tabs
- Insert **4** spaces for tabs to avoid avoid tab wars.
- Change your ide or editor to replace tabs for spaces.

## Line Spacing
- One line between gameplay namespace.
- One line of space between functions declarations in source and header.
- One line after each class scope section in header.
- Function call spacing:
  -  No space before bracket.
  - No space just inside brackets.
  - One space after each comma separating parameters.
```cpp
serializer->writeFloat("range", _range, LIGHT_RANGE);
```
- Conditional statement spacing:
  - One space after conditional keywords.
  - No space just inside the brackets.
  - One space separating commas, colons and condition comparison operators.
```cpp
    if (enumName.compare("gameplay::Light::Type") == 0)
    {
        switch (value)
        {
            case Light::TYPE_DIRECTIONAL:
                return "TYPE_DIRECTIONAL";
            ...
```
- **Do not** align blocks of variables to match spacing causing unnecessary line changes when new variables are introduced.
```cpp
private:
    int          foo;
    Vector3      bar;
    Ray          noonoo;
    unsigned int dirtyBits;
```

- Align indentation space for parameters when wrapping lines to match the initial bracket.

**Example:**
```cpp
Matrix::Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44)
```

**Example:**
```cpp
return sqrt((point.x - sphere.center.x) * (point.x - sphere.center.x) +
            (point.y - sphere.center.y) * (point.y - sphere.center.x) +
            (point.z - sphere.center.z) * (point.z - sphere.center.x));
```

- Use a line of space within .cpp implementation function to help organize blocks of code.

```cpp
// Lookup device surface extensions
 ...
 ...

// Create the platform surface connection
 ...
 ...
 ...
```

## Indentation
- Indent next line after all braces { }.
- Move code after braces { } to the next line.
- Always ident the next line of any condition statement line.

**Example**
```cpp
if (box.isEmpty())
    return;
```

**Example**
```cpp
for (size_t i = 0; i < count; ++i)
{
    if (distance(sphere, points[i]) > sphere.radius)
    {
        return false;
    }
}
```
- Never leave conditional code statements on same line as condition test:
```cpp
if (box.isEmpty()) return;
```
