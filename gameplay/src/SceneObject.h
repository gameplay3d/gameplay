#pragma once

#include "Serializable.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "BoundingSphere.h"
#include "Component.h"

class Camera;
class Light;
class GraphicsRenderer;
class PhysicsRigidBody;
class PhysicsCollider;
class Animation;
class Audio;
class Script;

namespace gameplay
{

/**
 * Defines an object that is part of a scene in the game.
 */
class SceneObject : public std::enable_shared_from_this<SceneObject>, public Serializable
{
    friend class Game;
    friend class Serializer::Activator;

public:
    /**
     * Constructor
     */
	SceneObject();

    /**
     * Destructor.
     */
	~SceneObject();

	/**
	 * Gets the name of this object.
	 *
	 * @return The name of this object.
	 */
	std::string getName() const;

	/**
	 * Sets the name of this object.
	 *
	 * @param name The name of this object.
	 */
	void setName(const std::string& name);

	/**
	 * Determines if the object is static (not moveable).
	 *
	 * @return true if the object is static, false if not.
	 */
	bool isStatic() const;

	/**
	 * Sets the object to be static (not moveable).
	 *
	 * Used by editor only.
	 *
	 * @param isStatic true if the object is static, false if not.
	 */
	void setStatic(bool isStatic);

    /**
     * Determines the local enabled state.
	 *
	 * An object may be disabled due to parent disabled state in hierarchy.
     * 
     * @return if the object is enabled in the scene.
	 * @see isEnabledInHierarchy to check if the object is actually enabled in scene.
     */
	bool isEnabled() const;

    /**
     * Sets if the object as enabled.
	 *
     * An object may be disabled due to parent disabled state in hierarchy.
	 * 
     * @param  true if the object is active, false if not active.
	 * @see isEnabledInHierarchy to check if the object is actually active in scene.
     */
	void setEnabled(bool enabled);

    /**
     * Resets the local position, eulerAngles/rotation and scale.
     */
    void resetLocalTransform();

	/**
	 * Gets the scale in local space.
	 *
	 * @return The scale in local space.
	 */ 
    const Vector3& getLocalScale() const;

	/**
	 * Sets the scale in local space.
	 *
	 * @param scale The scale in local space.
	 */ 
	void setLocalScale(const Vector3& scale);

	/**
	 * Gets the position in local space.
	 *
	 * @return The position in local space.
	 */ 
	const Vector3& getLocalPosition() const;

	/**
	 * Sets the position in local space.
	 *
	 * @param position The scale in local space.
	 */ 
	void setLocalPosition(const Vector3& position);

	/**
	 * Gets the rotation in local space as euler angles.
	 *
	 * @return The rotation in local space as euler angles.
	 */ 
	const Vector3& getLocalEulerAngles() const;

	/**
	 * Sets the rotation in local space as euler angles.
	 *
	 * @param eularAngles The rotation in local space as euler angles.
	 */ 
	void setLocalEulerAngles(const Vector3& eularAngles);

	/**
	 * Gets the rotation in local space as a quaternion.
	 *
	 * @return The rotation in local space as a quaternion.
	 */ 
    const Quaternion& getLocalRotation() const;

	/**
	 * Sets the rotation in local space as a quaternion.
	 *
	 * @param rotation The rotation in local space as a quaternion.
	 */ 
	void setLocalRotation(const Quaternion& rotation);

	/**
	 * Gets the position in world space.
	 *
	 * @return The position in world space.
	 */ 
    Vector3 getPosition();

	/**
	 * Sets the position in world space.
	 *
	 * @param position The scale in world space.
	 */ 
	void setPosition(const Vector3& position);

	/**
	 * Gets the rotation in world space as euler angles.
	 *
	 * @return The rotation in world space as euler angles.
	 */ 
	Vector3 getEulerAngles();

	/**
	 * Sets the rotation in world space as euler angles.
	 *
	 * @param eularAngles The rotation in world space as euler angles.
	 */ 
	void setEulerAngles(const Vector3& eularAngles);

	/**
	 * Gets the rotation in world space as a quaternion.
	 *
	 * @return The rotation in world space as a quaternion.
	 */ 
    Quaternion getRotation();

	/**
	 * Sets the rotation in world space as a quaternion.
	 *
	 * @param rotation The rotation in world space as a quaternion.
	 */ 
	void setRotation(const Quaternion& rotation);

	/**
	 * Translates this object in local space by the specified translation.
	 *
	 * @param translation The amount to translate.
	 */
	void translateLocal(const Vector3& translation);

	/**
	 * Translates this object in world space by the specified translation.
	 *
	 * @param translation The amount to translate.
	 */
	void translate(const Vector3& translation);

	/**
	 * Rotates this object in local space by the specified rotation.
	 *
	 * @param eularAngles The amount to rotate by (in degrees).
	 */
	void rotateLocal(const Vector3& eularAngles);

	/**
	 * Rotates this object in world space by the specified rotation.
	 *
	 * @param eularAngles The amount to rotate by (in degrees).
	 */
	void rotate(const Vector3& eularAngles);

	/**
	 * Orients the object so that the negative z axis points towards the target.
	 *
	 * @param target The world space target position to look at.
	 * @param worldUp The up vector 
	 */
	void lookAt(const Vector3& target, Vector3 worldUp = Vector3::up());

    /**
	 * Gets the normalized local space x-axis vector of the object in world space.
	 *
	 * @return The normalized local space x-axis vector of the object in world space.
	 */
    Vector3 getRight();

    /**
	 * Gets the normalized local space y-axis vector of the object in world space.
	 *
	 * @return The normalized local space y-axis vector of the object in world space.
	 */
    Vector3 getUp();

   /**
	 * Gets the normalized local space z-axis vector of the object in world space.
	 *
	 * @return The normalized local space z-axis vector of the object in world space.
	 */
    Vector3 getForward();

	/**
	 * Gets the matrix that transforms a point from local space into world space.
	 *
	 * @return The matrix that transforms a point from local space into world space.
	 */
	const Matrix& getWorldMatrix();

	/**
	 * Gets the matrix that transforms a point from world space into local space.
	 *
	 * @return The matrix that transforms a point from world space into local space.
	 */
	const Matrix& getWorldToLocalMatrix();

	/**
	 * Transforms a point in local space to world space.
	 * 
	 * @param p The point to be transformed.
	 * @param dst The transform point.
	 */
	void transformPoint(const Vector3& p, Vector3* dst);

	/**
	 * Transforms a vector in local space to world space.
	 * 
	 * @param v The vector to be transformed.
	 * @param dst The transform point.
	 */
    void transformVector(const Vector3& v, Vector3* dst);

    /**
     * Adds an object as a child of this object.
     *
     * @param object The object to add as a child of this object.
     */
    void addChild(std::shared_ptr<SceneObject> object);

    /**
     * Removes an object as a child of this object.
     *
     * @param object The object to removed.
     */
    void removeChild(std::shared_ptr<SceneObject> object);

    /**
     * Removes all children object of this object.
     */
    void removeChildren();

    /**
     * Gets the number of children this object has.
	 *
     * @return The number of children.
     */
    size_t getChildCount() const;

    /**
     * Gets the children object.
     *
     * The children are a copy.
     *
     * @return The children objects.
     */
    std::vector<std::shared_ptr<SceneObject>> getChildren();

    /**
     * Gets the parent of this object.
     *
     * @return The parent.
     */
    std::shared_ptr<SceneObject> getParent() const;

    /**
     * Finds the first child object that matches the given name.
     *
     * This method checks the specified name against its immediate children
     * but does not check the name against itself.
     * If recursive is true, it also traverses the objects's hierarchy with a breadth first search.
     *
     * @param name The name of the child object to find.
     * @param recursive true to search recursively all the object's children, false for only direct children.
     * @param exactMatch true if only objects whose name exactly matches the specified name are returned,
     *        or false if objects that start with the given name are returned.
     * @return The object found or nullptr if not found.
     */
    std::shared_ptr<SceneObject> findObject(const std::string& name, bool recursive = true, bool exactMatch = true);

    /**
     * Finds all child object that match the given name.
     *
     * @param name The name of the object to find.
     * @param objects A vector of objects to be populated with matches.
     * @param recursive true if a recursive search should be performed, false otherwise.
     * @param exactMatch true if only objects whose name exactly matches the specified name are returned,
     *        or false if objects that start with the given name are returned.
     * @return The number of matches found.
     */
    size_t findObjects(const std::string& name, std::vector<std::shared_ptr<SceneObject>>& objects, bool recursive = true, bool exactMatch = true);

    /**
     * Attaches a component to the object.
     *
     * @param component The component being attached to the object.
     */
    void attachComponent(std::shared_ptr<Component> component);

    /**
     * Detaches a component from the object.
     *
     * @param component The component being attached to the object.
     */
    void detachComponent(std::shared_ptr<Component> component);

    /**
     * Gets a componenent from the object for the class type.
     *
     * @return The component.
     */
    std::shared_ptr<Component> getComponent(Component::TypeId typeId);

    /**
     * Gets a componenent from the object for the class type.
     *
     * @return The components.
     */
    void getComponents(Component::TypeId typeId, std::vector<std::shared_ptr<Component>>& components);

    /**
     * Gets all the componenents from the object.
     *
     * @return The components.
     */
    void getComponents(std::vector<std::shared_ptr<Component>>& components);

    /**
     * Loads the scene and any required resources.
     */
    void load();

    /**
     * Unloads the scene resources.
     */
    void unload();

    /**
     * Checks if the scene resources are loaded.
     */
    bool isLoaded() const;

protected:

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

private:

	const Matrix& getLocalMatrix();
	void onInitialize();
	void onFinalize();
	void onUpdate(float elapsedTime);
	void onRender(float elapsedTime);

	std::string _name;
    bool _loaded;
	bool _enabled;
	bool _static;
	Vector3 _position;
    Quaternion _rotation;
	Vector3 _eulerAngles;
	Vector3 _scale;
	Matrix _localMatrix;
	Matrix _worldMatrix;
	Matrix _worldToLocalMatrix;
    int _dirtyBits;
	std::weak_ptr<SceneObject> _parent;
    std::vector<std::shared_ptr<SceneObject>> _children;
    std::vector<std::shared_ptr<Component>> _components;
};

}
