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
 */
class SceneLoader
{
    friend class Package;
    friend class PhysicsController;
    friend class Scene;

private:

    // Main interface to Scene::load(const char*).
    static Scene* load(const char* filePath);
    
    // ------------------------------------------------------------------------
    // Helper structures and functions for SceneLoader::load(const char*).

    struct MeshRigidBodyData
    {
        Mesh* mesh;
        unsigned char* vertexData;
        std::vector<unsigned char*> indexData;
    };

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
        enum Type { AUDIO, MATERIAL, PARTICLE, RIGIDBODY, TRANSLATE, ROTATE, SCALE, URL };

        SceneNodeProperty(Type type, const char* nodeID, std::string file, std::string id)
            : _type(type), _nodeID(nodeID), _file(file), _id(id) {}

        Type _type;
        const char* _nodeID;
        std::string _file;
        std::string _id;
    };

    static void addMeshRigidBodyData(std::string package, std::string id, Mesh* mesh, unsigned char* vertexData, unsigned int vertexByteCount);
    static void addMeshRigidBodyData(std::string package, std::string id, unsigned char* indexData, unsigned int indexByteCount);
    static void addSceneAnimation(const char* animationID, const char* targetID, const char* url);
    static void addSceneNodeProperty(SceneNodeProperty::Type type, const char* nodeID, const char* url = NULL);
    static void applyNodeProperties(const Scene* scene, const Properties* sceneProperties);
    static void applyNodeUrls(Scene* scene);
    static void buildReferenceTables(Properties* sceneProperties);
    static void calculateNodesWithMeshRigidBodies(const Properties* sceneProperties);
    static void createAnimations(const Scene* scene);
    static const MeshRigidBodyData* getMeshRigidBodyData(std::string id);
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

    // Holds all the node properties declared in the .scene file.
    static std::vector<SceneNodeProperty> _nodeProperties;

    // Holds the node IDs that need to be loaded with mesh rigid body support.
    static std::vector<std::string> _nodesWithMeshRB;

    // Stores the mesh data needed for triangle mesh rigid body support.
    static std::map<std::string, MeshRigidBodyData>* _meshRigidBodyData;

    // The path of the main GPB for the scene being loaded.
    static std::string _path;
};

}

#endif
