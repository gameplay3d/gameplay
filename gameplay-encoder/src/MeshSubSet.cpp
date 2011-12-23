#include "MeshSubSet.h"

namespace gameplay
{

MeshSubSet::MeshSubSet(void)
{
    
}


MeshSubSet::~MeshSubSet(void)
{
}

unsigned int MeshSubSet::getTypeId(void)
{
    return MESHPART_ID;
}
    
const char* MeshSubSet::getElementName(void)
{
    return "MeshSubSet";
}

void MeshSubSet::writeBinary(FILE* file)
{
    write(getTypeId(), file);

}
void MeshSubSet::writeText(FILE* file)
{
    fprintElementStart(file);

    fprintfElement(file, "%d ", "indices", indices);

    fprintElementEnd(file);
}

}