#include "ReferenceTable.h"

namespace gameplay
{

ReferenceTable::ReferenceTable(void)
{
}

ReferenceTable::~ReferenceTable(void)
{
}

void ReferenceTable::add(std::string xref, Object* obj)
{
    table[xref] = Reference(xref, obj);
}

Object* ReferenceTable::get(const std::string& xref)
{
    std::map<std::string, Reference>::iterator it = table.find(xref);
    if (it != table.end())
    {
        Reference ref = it->second;
        return ref.getObj();
    }
    return NULL;
}

void ReferenceTable::writeBinary(FILE* file)
{
    write(table.size(), file);
    for ( std::map<std::string, Reference>::iterator i=table.begin() ; i != table.end(); i++ )
    {
        i->second.writeBinary(file);
    }
}

void ReferenceTable::writeText(FILE* file)
{
    fprintf(file, "<RefTable>\n");
    for ( std::map<std::string, Reference>::iterator i=table.begin() ; i != table.end(); i++ )
    {
        i->second.writeText(file);
    }
    fprintf(file, "</RefTable>\n");
}

void ReferenceTable::updateOffsets(FILE* file)
{
    for (std::map<std::string, Reference>::iterator i = table.begin(); i != table.end(); i++)
    {
        Reference& ref = i->second;
        ref.updateOffset(file);
    }
}

std::map<std::string, Reference>::iterator ReferenceTable::begin()
{
    return table.begin();
}

std::map<std::string, Reference>::iterator ReferenceTable::end()
{
    return table.end();
}

}