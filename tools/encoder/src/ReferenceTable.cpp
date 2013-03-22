#include "Base.h"
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
    _table[xref] = Reference(xref, obj);
}

Object* ReferenceTable::get(const std::string& xref)
{
    std::map<std::string, Reference>::iterator it = _table.find(xref);
    if (it != _table.end())
    {
        Reference ref = it->second;
        return ref.getObj();
    }
    return NULL;
}

void ReferenceTable::writeBinary(FILE* file)
{
    write((unsigned int)_table.size(), file);
    for ( std::map<std::string, Reference>::iterator i=_table.begin() ; i != _table.end(); ++i)
    {
        i->second.writeBinary(file);
    }
}

void ReferenceTable::writeText(FILE* file)
{
    fprintf(file, "<RefTable>\n");
    for ( std::map<std::string, Reference>::iterator i=_table.begin() ; i != _table.end(); ++i)
    {
        i->second.writeText(file);
    }
    fprintf(file, "</RefTable>\n");
}

void ReferenceTable::updateOffsets(FILE* file)
{
    for (std::map<std::string, Reference>::iterator i = _table.begin(); i != _table.end(); ++i)
    {
        Reference& ref = i->second;
        ref.updateOffset(file);
    }
}

std::map<std::string, Reference>::iterator ReferenceTable::begin()
{
    return _table.begin();
}

std::map<std::string, Reference>::iterator ReferenceTable::end()
{
    return _table.end();
}

}
