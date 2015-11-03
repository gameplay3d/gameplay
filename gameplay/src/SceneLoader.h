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
 * Defines an internal helper class for loading scenes from .scene files.
 *
 * @script{ignore}
 */
class SceneLoader
{
    friend class Scene;

private:

    /**
     * Loads a scene using the data from the Properties object defined at the specified URL, 
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
     * 
     * @param url The URL pointing to the Properties object defining the scene.
     */
    static Scene* load(const char* url);
    
    /**
     * Helper structures and functions for SceneLoader::load(const char*).
     */
    struct SceneAnimation
    {
        SceneAnimation(const char* animationID, const char* targetID, std::string url)
            : _animationID(animationID), _targetID(targetID), _url(url) {}

        const char* _animationID;
        const char* _targetID;
        std::string _url;
    };

    struct SceneNodeProperty
    {
        enum Type
        {
            AUDIO = 1,
            MATERIAL = 2,
            PARTICLE = 4,
            TERRAIN = 8,
            LIGHT = 16,
            CAMERA = 32,
            COLLISION_OBJECT = 64,
            TRANSLATE = 128,
            ROTATE = 256,
            SCALE = 512,
            URL = 1024,
            SCRIPT = 2048,
            SPRITE = 4096,
            TILESET = 8192,
            TEXT = 16384,
            ENABLED = 32768
        };

        SceneNodeProperty(Type type, const std::string& value, int index, bool isUrl);

        Type _type;
        std::string _value;
        bool _isUrl;
        int _index;
    };

    struct SceneNode
    {
        SceneNode();

        const char* _nodeID;
        bool _exactMatch;
        Properties* _namespace;
        std::vector<Node*> _nodes; // list of nodes sharing properties defined in this SceneNode
        std::vector<SceneNode> _children; // list of unique child nodes
        std::vector<SceneNodeProperty> _properties;
        std::map<std::string, std::string> _tags;
    };

    SceneLoader();

    Scene* loadInternal(const char* url);

    void applyTags(SceneNode& sceneNode);

    void addSceneAnimation(const char* animationID, const char* targetID, const char* url);

    void addSceneNodeProperty(SceneNode& sceneNode, SceneNodeProperty::Type type, const char* value = NULL, bool supportsUrl = false, int index = 0);

    void applyNodeProperties(const Properties* sceneProperties, unsigned int typeFlags);

    void applyNodeProperties(SceneNode& sceneNode, const Properties* sceneProperties, unsigned int typeFlags);

    void applyNodeProperty(SceneNode& sceneNode, Node* node, const Properties* sceneProperties, const SceneNodeProperty& snp);

    void applyNodeUrls();

    void applyNodeUrls(SceneNode& sceneNode, Node* parent);

    void buildReferenceTables(Properties* sceneProperties);

    void parseNode(Properties* ns, SceneNode* parent, const std::string& path);

    void calculateNodesWithMeshRigidBodies(const Properties* sceneProperties);

    void createAnimations();

    PhysicsConstraint* loadGenericConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);

    PhysicsConstraint* loadHingeConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);

    Scene* loadMainSceneData(const Properties* sceneProperties);

    void loadPhysics(Properties* physics);

    void loadReferencedFiles();

    PhysicsConstraint* loadSocketConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);

    PhysicsConstraint* loadSpringConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB);

    std::map<std::string, Properties*> _propertiesFromFile; // Holds the properties object for a given file.
    std::map<std::string, Properties*> _properties;         // Holds the properties object for a given URL.
    std::vector<SceneAnimation> _animations;                // Holds the animations declared in the .scene file.
    std::vector<SceneNode> _sceneNodes;                     // Holds all the nodes+properties declared in the .scene file.
    std::string _gpbPath;                                   // The path of the main GPB for the scene being loaded.
    std::string _path;                                      // The path of the scene file being loaded.
    Scene* _scene;                                          // The scene being loaded
};

/**
 * Utility function for splitting up a URL of the form 'file#id', where one or both of file and id may be empty.
 * 
 * @param url The url to split.
 * @param file The out parameter containing the file part of the url.
 * @param id The out parameter containing the id part of the url.
 * @script{ignore}
 */
void splitURL(const std::string& url, std::string* file, std::string* id);

}

#endif
