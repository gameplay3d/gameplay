#ifndef SCENELOADER_H_
#define SCENELOADER_H_

#include "Base.h"
#include "Mesh.h"
#include "PhysicsRigidBody.h"
#include "Properties.h"
#include "Scene.h"

namespace gameplay
{

/**
 * Helper class for loading scenes from .scene files.
 *
 * @todo Add support for loading ghost objects and characters for nodes.
 * @todo Add support for explicitly specifying collision shapes for rigid bodies/ghost objects/characters.
 * @todo Consider supporting 'rigidbodymodel' on models/meshes that are not part of the scene to allow
 *		mesh data to be exported from a modelling tool for the sole purpose of representing a physics
 *		rigid body, but not have it get loaded into the scene and rendering context.
 */
class SceneLoader
{
    friend class Scene;

private:

    // Main interface to Scene::load(const char*).
    static Scene* load(const char* filePath);
    
    // ------------------------------------------------------------------------
    // Helper structures and functions for SceneLoader::load(const char*).

    struct SceneAnimation
    {
        SceneAnimation(const char* animationID, const char* targetID, std::string file, std::string id)
            : _animationID(animationID), _targetID(targetID), _file(file), _id(id) {}

        const char* _animationID;
        const char* _targetID;
        std::string _file;
        std::string _id;
    };

    struct SceneNodeProperty
    {
        enum Type
        {
            AUDIO = 1,
            MATERIAL = 2,
            PARTICLE = 4,
            RIGIDBODY = 8,
            TRANSLATE = 16,
            ROTATE = 32,
            SCALE = 64,
            URL = 128,
            TRANSPARENT = 256
        };

        SceneNodeProperty(Type type, std::string file, std::string id, int index) : _type(type), _file(file), _id(id), _index(index) { }

        Type _type;
        std::string _file;
        std::string _id;
        int _index;
    };

    struct SceneNode
    {
        SceneNode() : _nodeID(""), _exactMatch(true) { }

        const char* _nodeID;
        bool _exactMatch;
        std::vector<SceneNodeProperty> _properties;
    };

    static void addSceneAnimation(const char* animationID, const char* targetID, const char* url);
    static void addSceneNodeProperty(SceneNode& sceneNode, SceneNodeProperty::Type type, const char* url = NULL, int index = 0);
    static void applyNodeProperties(const Scene* scene, const Properties* sceneProperties, unsigned int typeFlags);
    static void applyNodeProperty(SceneNode& sceneNode, Node* node, const Properties* sceneProperties, const SceneNodeProperty& snp);
    static void applyNodeUrls(Scene* scene);
    static void buildReferenceTables(Properties* sceneProperties);
    static void calculateNodesWithMeshRigidBodies(const Properties* sceneProperties);
    static void createAnimations(const Scene* scene);
    static PhysicsConstraint* loadGenericConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);
    static PhysicsConstraint* loadHingeConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);
    static Scene* loadMainSceneData(const Properties* sceneProperties);
    static void loadPhysics(Properties* physics, Scene* scene);
    static void loadReferencedFiles();
    static PhysicsConstraint* loadSocketConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);
    static PhysicsConstraint* loadSpringConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);
    static void splitURL(const char* url, std::string* file, std::string* id);
    
    // ------------------------------------------------------------------------

    // Holds the properties object for a given file path.
    static std::map<std::string, Properties*> _propertiesFromFile;

    // Holds the animations declared in the .scene file.
    static std::vector<SceneAnimation> _animations;

    // Holds all the nodes+properties declared in the .scene file.
    static std::vector<SceneNode> _sceneNodes;


    // The path of the main GPB for the scene being loaded.
    static std::string _path;
};

}

#endif
