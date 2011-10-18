/*
 * Material.h
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Effect.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "Transform.h"

namespace gameplay
{

class MaterialParameter;

/**
 * Defines a material for an object to be rendered.
 *
 * This class encapsulates the parameters and logic required to apply a shader
 * to an object to be rendered. This includes specifying both a vertex and fragment
 * shader, as well as any uniforms and vertex attributes to be applied to these.
 */
class Material : public Ref
{
public:

    /**
     * Creates a material from the specified effect.
     *
     * @param effect The effect for the new material.
     * 
     * @return A new Material.
     */
    static Material* createMaterial(Effect* effect);

    /**
     * Creates a material using the specified vertex and fragment shader.
     *
     * @param vshPath The path to the vertex shader file.
     * @param fshPath The path to the fragment shader file.
     * @param defines A new-line delimited list of preprocessor defines. May be NULL.
     * 
     * @return A new Material.
     */
    static Material* createMaterial(const char* vshPath, const char* fshPath, const char* defines = NULL);

    /**
     * Returns the effect for this Material.
     * 
     * @return The effect for this Material.
     */
    Effect* getEffect();

    /**
     * Returns a Parameter for the specified material parameter name.
     *
     * The returned Parameter can be used to set values for the given
     * parameter name.
     *
     * @param name The material parameter (uniform) name.
     * 
     * @return A Parameter that can be set parameter values, or NULL if there is no
     *      uniform in the underlying effect with the given name.
     */
    MaterialParameter* getParameter(const char* name);

    /**
     * Binds this Material and any currently set material parameters.
     */
    void bind();

private:

    /**
     * Constructor.
     */
    Material();

    /**
     * Destructor.
     */
    ~Material();

    /**
     * Hidden copy constructor.
     */
    Material(const Material& m);

    Effect* _effect;
    std::vector<MaterialParameter*> _parameters;
};

}

// Include MaterialParameter after the Material class declaration
// to avoid an erroneous circular dependency during compilation.
#include "MaterialParameter.h"

#endif
