    #pragma once

#include "Component.h"
#include "Frustum.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Vector3.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines a camera which acts as a view of a scene to be rendered.
 */
class Camera : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    Camera();

    /**
     * Destructor.
     */
	~Camera();

    /**
     * The camera projection mode.
     */
    enum Mode
    {
        MODE_PERSPECTIVE,
        MODE_ORTHOGRAPHIC
    };

    /**
     * Gets the projection mode the camera is set to.
     *
     * @return The camera mode.
     */
    Camera::Mode getMode() const;

    /**
     * Set the projection mode that the camera is in.
     *
     * @param mode The mode that the camera is in.
     */
    void setMode(Camera::Mode mode);

    /**
     * Gets the field of view (perspective mode only).
     *
     * @return The field of view.
     */
    float getFieldOfView() const;

    /**
     * Sets the field of view (perspective mode only).
     * 
     * Normally in the range of 40-60 degrees.
     *
     * @param fieldOfView The field of view.
     */
    void setFieldOfView(float fieldOfView);

    /**
     * Gets the size (orthographic mode only).
     *
     * Default is 5.0
     *
     * @return The size.
     */
    float getSize() const;

    /**
     * Sets the size (orthographic mode only).
     *
     * Default is 5.0f.
     *
     * @param size The size.
     */
    void setSize(float size);

    /**
     * Gets the near z clipping plane distance.
     *
     * @return The near z clipping plane distance.
     */
    float getClipPlaneNear() const;

    /**
     * Sets the near z clipping plane distance.
     *
     * @param nearPlane The near z clipping plane distance.
     */
    void setClipPlaneNear(float nearPlane);

    /**
     * Gets the far z clipping plane distance.
     *
     * @return The far z clipping plane distance.
     */
    float getClipPlaneFar() const;

    /**
     * Sets the far z clipping plane distance.
     *
     * @param farPlane The far z clipping plane distance.
     */
    void setClipPlaneFar(float farPlane);

    /**
     * Resets the camera for the specified mode.
     *
     * @param mode The mode to reset the camera for.
     */
    void reset(Camera::Mode mode);

    /**
     * Gets the camera's view matrix.
     *
     * @return The camera view matrix.
     */
    const Matrix& getViewMatrix() const;

    /**
     * Gets the camera's inverse view matrix.
     *
     * @return The camera inverse view matrix.
     */
    const Matrix& getInverseViewMatrix() const;

    /**
     * Gets the camera's projection matrix.
     *
     * @return The camera projection matrix.
     */
    const Matrix& getProjectionMatrix() const;

    /**
     * Sets a custom projection matrix to be used by the camera.
     *
     * Setting a custom projection matrix results in the internally
     * computed projection matrix being completely overridden until
     * the resetProjectionMatrix method is called. A custom projection
     * matrix is normally not necessary, but can be used for special
     * projection effects, such as setting an oblique view frustum
     * for near plane clipping.
     *
     * @param matrix Custom projection matrix.
     */
    void setProjectionMatrix(const Matrix& matrix);

    /**
     * Gets the camera's view * projection matrix.
     *
     * @return The camera view * projection matrix.
     */
    const Matrix& getViewProjectionMatrix() const;

    /**
     * Gets the camera's inverse view * projection matrix.
     *
     * @return The camera inverse view * projection matrix.
     */
    const Matrix& getInverseViewProjectionMatrix() const;

    /**
     * Gets the view bounding frustum.
     *
     * @return The viewing bounding frustum.
     */
    const Frustum& getFrustum() const;

    /**
     * Projects the specified world position into the viewport coordinates.
     *
     * @param viewport The viewport rectangle to use.
     * @param position The world space position.
     * @param x The returned viewport x coordinate.
     * @param y The returned viewport y coordinate.
     * @param depth The returned pixel depth (can be nullptr).
     *
     * @script{ignore}
     */
    void project(const Rectangle& viewport, const Vector3& position, float* x, float* y, float* depth = nullptr) const;

    /**
     * Projects the specified world position into the viewport coordinates.
     *
     * @param viewport The viewport rectangle to use.
     * @param position The world space position.
     * @param out Populated with the resulting screen-space position.
     */
    void project(const Rectangle& viewport, const Vector3& position, Vector2* out) const;

    /**
     * Projects the specified world position into the viewport coordinates.
     *
     * @param viewport The viewport rectangle to use.
     * @param position The world space position.
     * @param out Populated with the resulting screen-space position, with the pixel depth in the Z coordinate.
     */
    void project(const Rectangle& viewport, const Vector3& position, Vector3* out) const;

    /**
     * Converts a viewport-space coordinate to a world-space position for the given depth value.
     *
     * The depth parameter is a value ranging between 0 and 1, where 0 returns a point on the
     * near clipping plane and 1 returns a point on the far clipping plane.
     *
     * @param viewport The viewport rectangle to use.
     * @param x The viewport-space x coordinate.
     * @param y The viewport-space y coordinate.
     * @param depth The depth range.
     * @param dst The world space position.
     */
    void unproject(const Rectangle& viewport, float x, float y, float depth, Vector3* dst) const;

    /**
     * Picks a ray that can be used for picking given the specified viewport-space coordinates.
     *
     * @param viewport The viewport rectangle to use.
     * @param x The viewport x-coordinate.
     * @param y The viewport y-coordinate.
     * @param dst The computed pick ray.
     */
    void pickRay(const Rectangle& viewport, float x, float y, Ray* dst) const;

    /**
     * @see Component::getTypeId
     */
    Component::TypeId getTypeId();

    /**
     * @see Serializable::getClassName
	 */
    std::string getClassName();

	/**
     * @see Serializable::onSerialize
	 */
    void onSerialize(Serializer* serializer);

	/**
     * @see Serializable::onDeserialize
	 */
    void onDeserialize(Serializer* serializer);

	/**
     * @see Serializer::Activator::CreateObjectCallback
	 */
    static std::shared_ptr<Serializable> createObject();
    
	/**
     * @see Serializer::Activator::EnumToStringCallback
	 */
    static std::string enumToString(const std::string& enumName, int value);

	/**
     * @see Serializer::Activator::EnumParseCallback
	 */
    static int enumParse(const std::string& enumName, const std::string& str);

protected:

    void setObject(std::shared_ptr<SceneObject> object);

private:

    Camera::Mode _mode;
    float _fieldOfView;
    float _size;
    float _clipPlaneNear;
    float _clipPlaneFar;
    float _aspectRatio;
    mutable Matrix _viewMatrix;
    mutable Matrix _projectionMatrix;
    mutable Matrix _viewProjectionMatrix;
    mutable Matrix _inverseViewMatrix;
    mutable Matrix _inverseViewProjectionMatrix;
    mutable Frustum _bounds;
    mutable int _dirtyBits;
};

}
