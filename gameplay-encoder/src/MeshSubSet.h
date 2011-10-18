#ifndef MESHSUBSET_H_
#define MESHSUBSET_H_

#include <vector>

#include "Common.h"
#include "Object.h"
#include "Vertex.h"

class MeshSubSet : public Object
{
public:
    MeshSubSet(void);
    virtual ~MeshSubSet(void);

    virtual byte getTypeId(void);
    virtual const char* getElementName(void);
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    std::vector<Vertex*> vertices;
    std::vector<int> indices;
    //Set<Vertex*, int> vertexLookupTable;
    std::map<Vertex, int> vertexLookupTable;
};


#endif
