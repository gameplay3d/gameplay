#pragma once

#include "Component.h"
#include "SceneObject.h"
#include "Geometry.h"
#include "Material.h"

namespace gameplay
{
    class Geometry;
    class Material;

/**
 * Defines an abstract renderer for rendering visual components.
 */
class Renderer : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;
public:

    /**
     * Constructor.
     */
    Renderer();

    /**
     * Destructor
     */
    virtual ~Renderer();

    /**
     * Gets the geometry for the renderer.
     *
     * @return The geometry for the index specified.
     */
    const Geometry& getGeometry() const;

    /**
     * Gets the number of materials included in the graphics component.
     *
     * @return The number of materials included in the graphics component. 
     */
    size_t getMaterialCount() const;

    /**
     * Gets the material for the index specified.
     *
     * @param index The geometry index.
     * @return The material for the index specified.
     */
    const Material& getMaterial(size_t index) const;

protected:

    Geometry _geometry;
    std::vector<Material> _materials;
};

}
