#ifndef DAEPOLYGONINPUT_H_
#define DAEPOLYGONINPUT_H_

#include <dom/domTypes.h>
#include <dom/domAccessor.h>

namespace gameplay
{

class DAEPolygonInput
{
public:
    DAEPolygonInput(void);
    virtual ~DAEPolygonInput(void);

    unsigned int offset;
    int type;
    domListOfFloats sourceValues;
    domAccessor* accessor;
};

}
#endif
