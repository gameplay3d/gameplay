#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include "Pass.h"

namespace gameplay
{

class Material;
class NodeCloneContext;

/**
 * Defines a technique for an object to be rendered.
 *
 * This class encapsulates a set of passes.
 */
class Technique : public RenderState
{
    friend class Material;
    friend class Pass;
    friend class RenderState;

public:

    /**
     * Gets the Id of this Material technique.
     */ 
    const char* getId() const;

    /**
     * Gets the number of passes in this technique.
     */
    unsigned int getPassCount() const;

    /**
     * Gets the pass at the specified index.
     */
    Pass* getPassByIndex(unsigned int index) const;

    /**
     * Gets the pass with the specified ID.
     */
    Pass* getPass(const char* id) const;

private:

    /**
     * Constructor.
     */
    Technique(const char* id, Material* material);

    /**
     * Hidden copy constructor.
     */
    Technique(const Technique&);

    /**
     * Destructor.
     */
    ~Technique();

    /**
     * Hidden copy assignment operator.
     */
    Technique& operator=(const Technique&);

    Technique* clone(Material* material, NodeCloneContext &context) const;

    std::string _id;
    Material* _material;
    std::vector<Pass*> _passes;
};

}

#endif
