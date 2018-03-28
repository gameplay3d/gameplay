#pragma once

#include "Serializable.h"
#include "SceneObject.h"
#include "Camera.h"

namespace gameplay
{

/**
 * Defines aa scene
 */
class Scene : public Serializable, public std::enable_shared_from_this<Scene>
{
    friend class Game;
    friend class Activator;

public:
    /**
     * Constructor
     */
    Scene();

    /**
     * Destructor.
     */
    ~Scene();

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
     * Determines if the scene is streaming (loads based on streaming distance from camera).
     *
     * @return true if the scene is streaming, false if not.
     */
    bool isStreaming() const;

    /**
     * Sets if the object is streaming (loads based on the active cameras visible range).
     *
     * @return true if the object is streaming, false if not.
     */
    void setStreaming(bool streaming);

    /**
     * Sets the active camera the current scene will use to view the scene.
     *
     * The active camera by default is the first object with a camera
     * attached called "mainCamera".
     *
     * The camera is not activated if the camera component is not
     * attached to an scene object in the active scene.
     *
     * @param camera The camera the scene will use to view the scene.
     */
    void setCamera(std::shared_ptr<Camera> camera);

    /**
     * Gets the active camera the current scene will use to view the scene.
     *
     * @return The camera the scene will use to view the scene.
     */
    std::shared_ptr<Camera> getCamera() const;

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
     * Clears all children objects from this object.
     */
    void clearChildren();

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
     * @see Activator::createObject
     */
    static std::shared_ptr<Serializable> createObject();

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

private:
    std::string _name;
    bool _streaming;
    std::shared_ptr<Camera> _camera;
    std::vector<std::shared_ptr<SceneObject>> _children;
    std::vector<std::shared_ptr<Component>> _components;
};

}
