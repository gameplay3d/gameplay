/*
 * GPBFile.h
 */

#ifndef GPBFILE_H_
#define GPBFILE_H_

#include <iostream>
#include <list>

#include "FileIO.h"
#include "Object.h"
#include "Scene.h"
#include "Node.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Reference.h"
#include "ReferenceTable.h"
#include "Animation.h"
#include "AnimationChannel.h"

namespace gameplay
{
    /**
     * Increment the version number when making a change that break binary compatibility.
     * [0] is major, [1] is minor.
     */
    const unsigned char VERSION[2] = {1, 0};

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
     * Saves the GPBFile as a binary file at filepath.
     *
     * @param filepath The file name and path to save to.
     */
    void saveBinary(const std::string& filepath);
    /**
     * Saves the GPBFile as a text file at filepath.
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

    /**
     * Adjusts the game play binary file before it is written.
     */
    void adjust();

    /**
     * Returns the animation that the given joint's animation channel's should be added to.
     * 
     * @param id The ID of the joint.
     * @return The animation belonging to the skin that the joint is part of.
     */
    Animation* findAnimationForJoint(const char* id);

private:

    FILE* file;
    std::list<Object*> objects;
    std::list<Camera*> cameras;
    std::list<Light*> lights;
    std::list<Mesh*> geometry;
    std::list<Node*> nodes; 
    ReferenceTable refTable;
};

}
#endif
