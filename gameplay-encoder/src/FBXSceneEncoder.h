#ifndef FBXSCENEEENCODER_H_
#define FBXSCENEEENCODER_H_

#ifdef USE_FBX

#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#ifdef WIN32
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4512 )
#endif
#include <fbxsdk.h>

#include "Base.h"
#include "StringUtil.h"
#include "Object.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshPart.h"
#include "MeshSkin.h"
#include "Model.h"
#include "Scene.h"
#include "Animation.h"
#include "AnimationChannel.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Transform.h"
#include "GPBFile.h"
#include "EncoderArguments.h"

using namespace gameplay;

/**
 * Class for binary encoding an FBX file.
 */
class FBXSceneEncoder
{
public:

    static const unsigned int SCENE_SKIN_VERTEXINFLUENCES_MAX = 4;
    
    /**
     * Constructor.
     */
    FBXSceneEncoder();

    /**
     * Destructor.
     */
    ~FBXSceneEncoder();
    
    /**
     * Writes out encoded FBX file.
     */
    void write(const std::string& filepath, const EncoderArguments& arguments);

private:

    /**
     * Loads the scene.
     * 
     * @param fbxScene The FBX scene to load.
     */
    void loadScene(KFbxScene* fbxScene);

    /**
     * Loads all of the animatiosn from the given FBX scene.
     * 
     * @param fbxScene The scene to load animations from.
     * @param arguments The command line arguments passed to the encoder.
     */
    void loadAnimations(KFbxScene* fbxScene, const EncoderArguments& arguments);

    /**
     * Loads the animations from the given FBX animation layer recursively starting from fbxNode.
     * 
     * @param fbxAnimLayer The FBX animation layer to load from.
     * @param fbxNode The node to start loading animations from.
     * @param arguments The command line arguments passed to the encoder.
     */
    void loadAnimationLayer(KFbxAnimLayer* fbxAnimLayer, KFbxNode* fbxNode, const EncoderArguments& arguments);

    /**
     * Loads animation channels from the given node and adds the channels to the given animation.
     * 
     * @param pAnimLayer The FBX animation layer to load from.
     * @param fbxNode The node to load animation channels from.
     * @param animation The animation to add the channels to.
     */
    void loadAnimationChannels(KFbxAnimLayer* pAnimLayer, KFbxNode* fbxNode, Animation* animation);

    /**
     * Loads the bind shape for all mesh skins that have be loaded so far.
     * 
     * @param fbxScene The FBX scene to read the bind shapes from.
     */
    void loadBindShapes(KFbxScene* fbxScene);

    /**
     * Loads the camera from the given FBX node and adds to it to the given GamePlay node.
     * 
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadCamera(KFbxNode* fbxNode, Node* node);

    /**
     * Loads the light from the given FBX node and adds to it to the given GamePlay node.
     * 
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadLight(KFbxNode* fbxNode, Node* node);
    
    /**
     * Loads the model from the given FBX node and adds to it to the given GamePlay node.
     *
     * @param fbxNode The FBX node to load from.
     * @param node The GamePlay node to add to.
     */
    void loadModel(KFbxNode* fbxNode, Node* node);

    /**
     * Loads the mesh skin from the given FBX mesh and adds it to the given GamePlay model.
     *
     * @param fbxMesh The FBX mesh to load the skin from.
     * @param model The model to add the skin to.
     */
    void loadSkin(KFbxMesh* fbxMesh, Model* model);
    
    /**
     * Loads the FBX Node and creates a GamePlay Node.
     * 
     * @param fbxNode The FBX Node to load.
     * 
     * @return The newly created Node or NULL if the node could not be loaded.
     */
    Node* loadNode(KFbxNode* fbxNode);
    
    /**
     * Loads the FbxMesh and returns a GamePlay mesh.
     * If the fbxMesh has already been loaded then the same instance of mesh will be returned.
     * 
     * @param fbxMesh The FBX Mesh to load.
     * 
     * @return The GamePlay mesh that was loaded from the FBX Mesh.
     */
    Mesh* loadMesh(KFbxMesh* fbxMesh);

    /**
     * Gets the Mesh that was saved with the given ID. Returns NULL if a match is not found.
     * 
     * @param meshId The ID of the FbxMesh to search for.
     * 
     * @return The mesh that was saved with the ID or NULL if none was found.
     */
    Mesh* getMesh(size_t meshId);

    /**
     * Saves the Mesh with the given id.
     * 
     * @param meshId The ID of the FbxMesh to use as a key.
     * @param mesh The mesh to save.
     */
    void saveMesh(size_t meshId, Mesh* mesh);
    
    /**
     * Prints a message.
     *
     * @param str The string to print.
     */
    void print(const char* str);

    /**
     * Transforms the GamePlay Node using the transform data from the FBX Node.
     * 
     * @param fbxNode The FBX node to get the transfrom data from
     * @param node The GamePlay Node to copy the transform to.
     */
    void transformNode(KFbxNode* fbxNode, Node* node);

    /**
     * Recursively triangules the meshes starting from the given node.
     * 
     * @param fbxNode The node to start triangulating from.
     */
    static void triangulateRecursive(KFbxNode* fbxNode);

    /**
     * Prints a warning message.
     */
    static void warning(const std::string& message);

    /**
     * Prints a warning message.
     */
    static void warning(const char* message);

private:

    /**
     * The GamePlay file that is populated while reading the FBX file.
     */
    GPBFile _gamePlayFile;

    /**
     * The collection of meshes for the purpose of making sure that the same model is not loaded twice. (Mesh instancing)
     */
    std::map<size_t, Mesh*> _meshes;

    /**
     * The animation that channels should be added to it the user is using the -groupAnimation command line argument. May be NULL.
     */
    Animation* _groupAnimation;
};

#endif
#endif