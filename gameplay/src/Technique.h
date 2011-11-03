/*
 * Technique.h
 */

#ifndef TECHNIQUE_H_
#define TECHNIQUE_H_

#include "Pass.h"

namespace gameplay
{

class Material;

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
     * Returns the Id of this Material technique.
     */ 
    const char* getId() const;

    /**
     * Returns the number of passes in this technique.
     */
    unsigned int getPassCount() const;

    /**
     * Returns the pass at the specified index.
     */
    Pass* getPass(unsigned int index) const;

    /**
     * Returns the pass with the specified ID.
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
    Technique(const Technique& t);

    /**
     * Destructor.
     */
    ~Technique();

    std::string _id;
    Material* _material;
    std::vector<Pass*> _passes;
};

}

#endif
