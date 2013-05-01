#ifndef GPBFILE_H_
#define GPBFILE_H_

#include "FileIO.h"
#include "Object.h"
#include "Scene.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Reference.h"
#include "ReferenceTable.h"
#include "Animations.h"
#include "Animation.h"
#include "AnimationChannel.h"

namespace gameplay
{

/**
 * Increment the version number when making a change that break binary compatibility.
 * [0] is major, [1] is minor.
 */
const unsigned char GPB_VERSION[2] = {1, 2};

/**
 * The GamePlay Binary file class handles writing the GamePlay Binary file.
 */
class GPBFile
{
public:

    /**
     * Constructor.
     */
    GPBFile(void);

    /**
     * Destructor.
     */
    ~GPBFile(void);

    /**
     * Returns the GPBFile instance.
     */
    static GPBFile* getInstance();

    /**
     * Saves the GPBFile as a binary file at filepath.
     *
     * @param filepath The file name and path to save to.
     * 
     * @return True if successful, false if error.
     */
    bool saveBinary(const std::string& filepath);

    /**
     * Saves the GPBFile as a text file at filepath. Useful for debugging.
     *
     * @param filepath The file name and path to save to.
     * 
     * @return True if successful, false if error.
     */
    bool saveText(const std::string& filepath);
    
    void add(Object* obj);
    void addScene(Scene* scene);
    void addCamera(Camera* camera);
    void addLight(Light* light);
    void addMesh(Mesh* mesh);
    void addNode(Node* node);
    /**
     * Adds a node that does not belong to a scene.
     */
    void addScenelessNode(Node* node);
    void addAnimation(Animation* animation);

    /**
     * Adds the given object to the ref table.
     */
    void addToRefTable(Object* obj);

    /**
     * Returns the object with the given id. Returns NULL if not found.
     */
    Object* getFromRefTable(const std::string& id);

    /**
    * Returns true if the id was found in the ref table.
    */
    bool idExists(const std::string& id);

    Camera* getCamera(const char* id);
    Light* getLight(const char* id);
    Mesh* getMesh(const char* id);
    Node* getNode(const char* id);

    Animations* getAnimations();

    /**
     * Returns the number of lights.
     */
    unsigned int getLightCount() const;

    /**
     * Adjusts the game play binary file before it is written.
     */
    void adjust();

    /**
     * Groups the animations of all mesh skins to be under one animation per mesh skin.
     */
    void groupMeshSkinAnimations();

    /**
     * Renames the animations in the list of animation ids to the new animation id.
     * 
     * @param animationIds The list of animations to rename.
     * @param newId The new animation id.
     */
    void renameAnimations(std::vector<std::string>& animationIds, const char* newId);

private:

    /**
     * Computes the bounds of all meshes in the node hierarchy.
     */
    void computeBounds(Node* node);

    /**
     * Optimizes animation data by removing unneccessary channels and keyframes.
     */
    void optimizeAnimations();

    /**
     * Decomposes an ANIMATE_SCALE_ROTATE_TRANSLATE channel into 3 new channels. (Scale, Rotate and Translate)
     * 
     * @param animation The animation that the channel belongs to.
     * @param channel The animation channel to decompose.
     * @param channelIndex Index of the channel.
     */
    void decomposeTransformAnimationChannel(Animation* animation, AnimationChannel* channel, int channelIndex);

    /**
     * Moves the animation channels that target the given node and its children to be under the given animation.
     * 
     * @param node The node to recursively search from.
     * @param animation The animation to move the channels to.
     */
    void moveAnimationChannels(Node* node, Animation* animation);

private:

    FILE* _file;
    std::list<Object*> _objects;
    std::list<Camera*> _cameras;
    std::list<Light*> _lights;
    std::list<Mesh*> _geometry;
    /**
     * The flat list of all nodes.
     */
    std::list<Node*> _nodes;
    Animations _animations;
    bool _animationsAdded;

    ReferenceTable _refTable;
};

}

#endif
