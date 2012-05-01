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
     */
    void saveBinary(const std::string& filepath);

    /**
     * Saves the GPBFile as a text file at filepath. Useful for debugging.
     *
     * @param filepath The file name and path to save to.
     */
    void saveText(const std::string& filepath);
    
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
     * Adjusts the game play binary file before it is written.
     */
    void adjust();

private:
    /**
     * Computes the bounds of all meshes in the node hierarchy.
     */
    void computeBounds(Node* node);
    void optimizeTransformAnimations();

    /**
     * Decomposes an ANIMATE_SCALE_ROTATE_TRANSLATE channel into 3 new channels. (Scale, Rotate and Translate)
     * 
     * @param animation The animation that the channel belongs to.
     * @param channel The animation channel to decompose.
     */
    void decomposeTransformAnimationChannel(Animation* animation, const AnimationChannel* channel);

private:

    FILE* _file;
    std::list<Object*> _objects;
    std::list<Camera*> _cameras;
    std::list<Light*> _lights;
    std::list<Mesh*> _geometry;
    std::list<Node*> _nodes;
    Animations _animations;
    bool _animationsAdded;

    ReferenceTable _refTable;
};

}

#endif
