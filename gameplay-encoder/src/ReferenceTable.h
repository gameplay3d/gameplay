#ifndef REFTABLE_H_
#define REFTABLE_H_

#include "FileIO.h"
#include "Reference.h"
#include "Object.h"

namespace gameplay
{

/**
 * Collection of unique Reference objects stored in a hashtable.
 */
class ReferenceTable
{
public:

    /**
     * Constructor.
     */
    ReferenceTable(void);

    /**
     * Destructor.
     */
    virtual ~ReferenceTable(void);

    /**
     * Adds an object to the reference table.
     * 
     * @param xref The xref for the object.
     * @param obj The object to be added.
     */
    void add(std::string xref, Object* obj);

    Object* get(const std::string& xref);

    void writeBinary(FILE* file);
    void writeText(FILE* file);

    /**
     * Updates the file positon offsets of the Reference objects in the GamePlay binary file.
     * This needs to be called after all of the objects have been written.
     * 
     * @param file The file pointer.
     */
    void updateOffsets(FILE* file);

    std::map<std::string, Reference>::iterator begin();
    std::map<std::string, Reference>::iterator end();

private:
    std::map<std::string, Reference> _table;
};

}

#endif
