#ifndef REFTABLE_H_
#define REFTABLE_H_

#include <map>

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

    void add(std::string xref, Object* obj);
    Object* get(const std::string& xref);

    void writeBinary(FILE* file);
    void writeText(FILE* file);

    void updateOffsets(FILE* file);

    std::map<std::string, Reference>::iterator begin();
    std::map<std::string, Reference>::iterator end();

private:
    std::map<std::string, Reference> table;
};

}
#endif

