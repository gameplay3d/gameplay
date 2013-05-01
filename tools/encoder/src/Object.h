#ifndef OBJ_H_
#define OBJ_H_

#include "FileIO.h"
#include "Base.h"

namespace gameplay
{

/**
 * Object is the abstract base class of all the objects that can be written in the GamePlay Binary file.
 */
class Object
{
public:

    // TypeID's
    enum TypeID
    {
        SCENE_ID = 1,
        NODE_ID = 2,
        ANIMATIONS_ID = 3,
        ANIMATION_ID = 4,
        ANIMATIONCHANNEL_ID = 5,
        NODEINSTANCE_ID = 8,
        MODEL_ID = 11,
        MATERIAL_ID = 16,
        EFFECT_ID = 17,
        CAMERA_ID = 32,
        LIGHT_ID = 33,
        MESH_ID = 34,
        MESHPART_ID = 35,
        MESHSKIN_ID = 36,
        FONT_ID = 128,
    };

    /**
     * Constructor.
     */
    Object(void);

    /**
     * Destructor.
     */
    virtual ~Object(void);

    /**
     * Returns the Object TypeID.
     */
    virtual unsigned int getTypeId(void) const;

    /**
     * Returns the string element name of the object.
     * Used for printing the gameplayfile as text.
     */
    virtual const char* getElementName(void) const = 0;

    /**
     * Writes this object to the file stream as binary.
     */
    virtual void writeBinary(FILE* file);

    /**
     * Writes this object to the file stream as text.
     */
    virtual void writeText(FILE* file) = 0;

    /**
     * Returns this object's id string.
     */
    const std::string& getId() const;

    /**
     * Sets this object's id string.
     */
    void setId(const char* id);

    /**
     * Sets this object's id string.
     */
    void setId(const std::string& id);

    /**
     * Prints an XML start element with the name of this object to the text file stream.
     * Also prints the id as an attribute if the id length is greater than zero.
     */
    void fprintElementStart(FILE* file);

    /**
     * Prints an XML end element with the name of this object to the text file stream.
     */
    void fprintElementEnd(FILE* file);

    /**
     * Writes the xref of this object to the binary file stream.
     */
    void writeBinaryXref(FILE* file);

    /**
     * Returns the file position that this object was written to.
     * An offset of zero means this object has not been written yet.
     */
    unsigned int getFilePosition();

    /**
     * Writes out a list of objects to a binary file stream.
     */
    template <class T>
    static void writeBinaryObjects(std::list<T> list, FILE* file)
    {
        // First write the size of the list
        write((unsigned int)list.size(), file);
        // Then write each element
        typename std::list<T>::const_iterator i;
        for (i = list.begin(); i != list.end(); ++i)
        {
            (*i)->writeBinary(file);
        }
    }

    /**
     * Writes out a vector of objects to a binary file stream.
     */
    template <class T>
    static void writeBinaryObjects(std::vector<T> vector, FILE* file)
    {
        // First write the size of the vector
        write((unsigned int)vector.size(), file);
        // Then write each element
        typename std::vector<T>::const_iterator i;
        for (i = vector.begin(); i != vector.end(); ++i)
        {
            (*i)->writeBinary(file);
        }
    }

private:

    /**
     * Saves where this object was written to in the binary file.
     */
    void saveFilePosition(FILE* file);

private:
    std::string _id;
    long _fposition;
};

}

#endif
