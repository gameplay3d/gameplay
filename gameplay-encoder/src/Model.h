#ifndef MODEL_H_
#define MODEL_H_

#include "Object.h"
#include "Mesh.h"
#include "MeshSkin.h"
#include "Material.h"

namespace gameplay
{

class Model : public Object
{
public:

    /**
     * Constructor.
     */
    Model(void);

    /**
     * Destructor.
     */
    virtual ~Model(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    Mesh* getMesh();
    void setMesh(Mesh* mesh);
    MeshSkin* getSkin();
    void setSkin(MeshSkin* skin);

private:

    Mesh* _mesh;
    MeshSkin* _meshSkin;
    std::list<Material*> _materials;
};

}

#endif
