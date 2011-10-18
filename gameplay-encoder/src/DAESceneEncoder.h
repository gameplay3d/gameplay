/*
 * DAESceneEncoder.h
 */

#ifndef DAESCENEEENCODER_H_
#define DAESCENEEENCODER_H_

#include <iostream>
#include <list>
#include <vector>

#include <dae.h>
#include <dae/daeSIDResolver.h>
#include <dae/domAny.h>
#include <dom/domCOLLADA.h>
#include <dom/domConstants.h>
#include <dom/domElements.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domCamera.h>

#include "Base.h"
#include "StringUtil.h"
#include "Object.h"
#include "Node.h"
#include "Camera.h"
#include "CameraInstance.h"
#include "Light.h"
#include "LightInstance.h"
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
#include "DAEChannelTarget.h"
#include "GPBFile.h"

using namespace gameplay;

/**
 * Class for binary encoding a Collada (DAE) file.
 */
class DAESceneEncoder
{
public:

    static const unsigned int SCENE_SKIN_VERTEXINFLUENCES_MAX = 4;
    
    /**
     * Constructor.
     */
    DAESceneEncoder();
    
    /**
     * Destructor.
     */
    ~DAESceneEncoder();
    
    /**
     * Writes out encoded Collada 1.4 file.
     */
    void write(const std::string& filepath, const char* nodeId, bool text);

private:

    class DAEPolygonInput
    {
    public:
        DAEPolygonInput(void);
        /**
         * Destructor.
         */
        virtual ~DAEPolygonInput(void);

        unsigned int offset;
        int type;
        domListOfFloats sourceValues;
        domAccessor* accessor;
    };

    class SkinnedVertexWeightPair
    {
    public:
        float BlendWeight;
        unsigned int BlendIndex;

        SkinnedVertexWeightPair(float blendWeight, unsigned int blendIndex) : BlendWeight(blendWeight), BlendIndex(blendIndex)
        {                
        }            

        bool operator < (const SkinnedVertexWeightPair& value) const
        {
            return value.BlendWeight < BlendWeight;
        }
    };
    
    void triangulate(DAE* dae);
    
    void createTrianglesFromPolygons(domMesh* domMesh, domPolygons* domPolygons);
    void createTrianglesFromPolylist(domMesh* domMesh, domPolylist* domPolylist);

    Node* loadNode(domNode* node, Node* parent);

    void loadScene(const domVisual_scene* visualScene);
    void loadCameraInstance(const domNode* n, Node* node);
    void loadControllerInstance(const domNode* n, Node* node);
    void loadLightInstance(const domNode* n, Node* node);
    void loadGeometryInstance(const domNode* n, Node* node);

    /**
     * Loads all animations from the given COLLADA dom into the GamePlayFile.
     */
    void loadAnimations(const domCOLLADA* dom);

    /**
     * Loads a COLLADA animation element.
     * 
     * @param animationRef Pointer to the animation dom element to load from.
     */
    void loadAnimation(const domAnimation* animationRef);

    CameraInstance* loadCamera(const domCamera* cameraRef);
    LightInstance* loadLight(const domLight* lightRef);
    Model* loadSkin(const domSkin* skinElement);
    Model* loadGeometry(const domGeometry* geometry, const domBind_materialRef bindMaterial);

    void loadSkeleton(domInstance_controller::domSkeleton* skeletonElement, MeshSkin* skin);
    
    /**
     * Loads interpolation curve data from the given source into the animation.
     * 
     * @param source The source dom element to load interpolation curves from.
     * @param animation The destination animation to copy to.
     */
    void loadInterpolation(const domSource* source, AnimationChannel* animation);

    /**
     * Returns the active camera node for the given scene.
     * 
     * @param visualScene The collada visual scene node.
     * @param scene The gameplay scene node.
     * @return The active camera node or NULL if one was not found.
     */
    Node* findSceneActiveCameraNode(const domVisual_scene* visualScene, Scene* scene);

    /**
     * Loads and returns a mesh (geometry). If the mesh has already been loaded, it is simply returned.
     */
    Mesh* loadMesh(const domMesh* meshElement, const std::string& geometryId);

    /**
     * Sets the transform of node from the domNode transform.
     */
    void transformNode(domNode* domNode, Node* node);

    /**
     * Calculates the transform from a domNode. (<translate>, <rotate>, <scale>)
     */
    void calcTransform(domNode* domNode, Matrix& dstTransform);

    /**
     * Gets the joint names for the given source and appends them to the given list.
     */
    void getJointNames(domSource* source, std::list<std::string>& list);

    void warning(const std::string& message);
    void warning(const char* message);

    /**
     * Loads the target data into the animation from the given channel's target.
     * Example: <channel target="Cube/location.X" />
     * Target ID is "Cube"
     * The target attribute is "location.X"
     * 
     * @param channelRef Pointer to the channel dom element.
     * @param animation The animation to copy to.
     */
    void loadTarget(const domChannel* channelRef, AnimationChannel* animation);

    /**
     * Finds the ID for an animation.
     * If the COLLADA animation element doesn't have an ID then this method will trying 
     * to find an appropriate ID for the animation.
     * 
     * @param animationRef The COLLADA animation element to find an ID for.
     * @return The ID string for the animation.
     */
    std::string findAnimationId(const domAnimation* animationRef);

    /**
     * Copies float values from a domFloat_array to a std::vector<float>.
     * 
     * @param source Source of float values to copy from.
     * @param target Destination float vector to copy to.
     */
    static void copyFloats(const domFloat_array* source, std::vector<float>* target);

    /**
     * Returns the VertexUsage value for the given semantic string.
     * 
     * @param semantic The semantic attribute string from the COLLADA <input> element.
     * @param The VertexUsage or -1 if the string was not recognized.
     */
    static int getVertexUsageType(const std::string& semantic);
    
    DAE* _collada;        // Collada datastore in memory to read from.
    FILE* file;        // Output file to write to.
    GPBFile _gamePlayFile;

    std::map<std::string, int> jointLookupTable;
    std::vector<Matrix>jointInverseBindPoseMatrices;
    float* vertexBlendWeights;
    unsigned int* vertexBlendIndices;

};

#endif
