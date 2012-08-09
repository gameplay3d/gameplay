#ifndef DAESCENEEENCODER_H_
#define DAESCENEEENCODER_H_

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
#include "DAEChannelTarget.h"
#include "GPBFile.h"
#include "DAEUtil.h"
#include "EncoderArguments.h"

namespace gameplay
{

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
    void write(const std::string& filepath, const EncoderArguments& arguments);

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

    /**
     * Optimizes the COLLADA dom based on the arguments passed to the encoder.
     * 
     * @param arguments The command line arguments passed to the encoder.
     * @param dom The COLLADA dom.
     */
    void optimizeCOLLADA(const EncoderArguments& arguments, domCOLLADA* dom);
    
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
     * @param animationRef The animation dom element to load from.
     * @param altId   The id string to use if the animation doesn't have an id.
     */
    void loadAnimation(const domAnimationRef animationRef, const char* altId = NULL);

    Camera* loadCamera(const domCamera* cameraRef);
    Light* loadLight(const domLight* lightRef);
    Model* loadSkin(const domSkin* skinElement);
    Model* loadGeometry(const domGeometry* geometry, const domBind_materialRef bindMaterial);

    void loadSkeleton(domNode* rootNode, MeshSkin* skin);
    void loadSkeleton(domInstance_controller::domSkeleton* skeletonElement, MeshSkin* skin);
    
    /**
     * Loads interpolation curve data from the given source into the animation.
     * 
     * @param source The source dom element to load interpolation curves from.
     * @param animation The destination animation to copy to.
     */
    void loadInterpolation(const domSourceRef source, AnimationChannel* animation);

    /**
     * Returns the active camera node for the given scene.
     * 
     * @param visualScene The collada visual scene node.
     * @param scene The gameplay scene node.
     * 
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
     * Loads the target data into the animation from the given channel's target.
     * Example: <channel target="Cube/location.X" />
     * Target ID is "Cube"
     * The target attribute is "location.X"
     * 
     * @param channelRef The channel dom element.
     * @param animationChannel The animation to copy to.
     * 
     * @return True if the animation channel data was loaded without error; false if there was an error.
     */
    bool loadTarget(const domChannelRef& channelRef, AnimationChannel* animationChannel);

    void begin();
    void end(const char* str);

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
     * 
     * @return The VertexUsage or -1 if the string was not recognized.
     */
    static int getVertexUsageType(const std::string& semantic);
    
private:
    
    DAE* _collada;        // Collada datastore in memory to read from.
    domCOLLADA* _dom;
    FILE* file;        // Output file to write to.
    GPBFile _gamePlayFile;

    std::map<std::string, int> _jointLookupTable;
    std::vector<Matrix>_jointInverseBindPoseMatrices;
    float* _vertexBlendWeights;
    unsigned int* _vertexBlendIndices;

    std::vector<std::string> _tempGroupAnimationIds;

    clock_t _begin;
};

}

#endif
