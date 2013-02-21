#include "Base.h"
#include "AudioListener.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "MeshSkin.h"
#include "Joint.h"
#include "Terrain.h"
#include "Bundle.h"

namespace gameplay
{

// Global list of active scenes
static std::vector<Scene*> __sceneList;

static inline char lowercase(char c)
{
    if (c >= 'A' && c <='Z')
    {
        c |= 0x20;
    }
    return c;
}

// Returns true if 'str' ends with 'suffix'; false otherwise.
static bool endsWith(const char* str, const char* suffix, bool ignoreCase)
{
    if (str == NULL || suffix == NULL)
        return false;
    size_t length = strlen(str);
    size_t suffixLength = strlen(suffix);

    if (suffixLength > length)
    {
        return false;
    }

    size_t offset = length - suffixLength;

    const char* p = str + offset;
    while (*p != '\0' && *suffix != '\0')
    {
        if (ignoreCase)
        {
            if (lowercase(*p) != lowercase(*suffix))
            {
                return false;
            }
        }
        else if (*p != *suffix)
        {
            return false;
        }
        
        ++p;
        ++suffix;
    }
    return true;
}


Scene::Scene(const char* id)
    : _id(id ? id : ""), _activeCamera(NULL), _firstNode(NULL), _lastNode(NULL), _nodeCount(0), 
    _lightColor(1,1,1), _lightDirection(0,-1,0), _bindAudioListenerToCamera(true), _debugBatch(NULL)
{
    __sceneList.push_back(this);
}

Scene::~Scene()
{
    // Unbind our active camera from the audio listener
    if (_activeCamera)
    {
        AudioListener* audioListener = AudioListener::getInstance();
        if (audioListener && (audioListener->getCamera() == _activeCamera))
        {
            audioListener->setCamera(NULL);
        }

        SAFE_RELEASE(_activeCamera);
    }

    // Remove all nodes from the scene
    removeAllNodes();
    SAFE_DELETE(_debugBatch);

    // Remove the scene from global list
    std::vector<Scene*>::iterator itr = std::find(__sceneList.begin(), __sceneList.end(), this);
    if (itr != __sceneList.end())
        __sceneList.erase(itr);
}

Scene* Scene::create(const char* id)
{
    return new Scene(id);
}

Scene* Scene::load(const char* filePath)
{
    if (endsWith(filePath, ".gpb", true))
    {
        Scene* scene = NULL;
        Bundle* bundle = Bundle::create(filePath);
        if (bundle)
        {
            scene = bundle->loadScene();
            SAFE_RELEASE(bundle);
        }
        return scene;
    }
    return SceneLoader::load(filePath);
}

Scene* Scene::getScene(const char* id)
{
    if (id == NULL)
        return __sceneList.size() ? __sceneList[0] : NULL;

    for (size_t i = 0, count = __sceneList.size(); i < count; ++i)
    {
        if (__sceneList[i]->_id == id)
            return __sceneList[i];
    }

    return NULL;
}

const char* Scene::getId() const
{
    return _id.c_str();
}

void Scene::setId(const char* id)
{
    _id = id ? id : "";
}

Node* Scene::findNode(const char* id, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);

    // Search immediate children first.
    for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            return child;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
        {
            Node* match = child->findNode(id, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }

    return NULL;
}

unsigned int Scene::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);

    unsigned int count = 0;

    // Search immediate children first.
    for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            nodes.push_back(child);
            ++count;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
        {
            count += child->findNodes(id, nodes, true, exactMatch);
        }
    }

    return count;
}

void Scene::visitNode(Node* node, const char* visitMethod)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // Invoke the visit method for this node.
    if (!sc->executeFunction<bool>(visitMethod, "<Node>", node))
        return;

    // If this node has a model with a mesh skin, visit the joint hierarchy within it
    // since we don't add joint hierarcies directly to the scene. If joints are never
    // visited, it's possible that nodes embedded within the joint hierarchy that contain
    // models will never get visited (and therefore never get drawn).
    if (node->_model && node->_model->_skin && node->_model->_skin->_rootNode)
    {
        visitNode(node->_model->_skin->_rootNode, visitMethod);
    }

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        visitNode(child, visitMethod);
    }
}

Node* Scene::addNode(const char* id)
{
    Node* node = Node::create(id);
    GP_ASSERT(node);
    addNode(node);

    // Call release to decrement the ref count to 1 before returning.
    node->release();

    return node;
}

void Scene::addNode(Node* node)
{
    GP_ASSERT(node);

    if (node->_scene == this)
    {
        // The node is already a member of this scene.
        return;
    }

    node->addRef();

    // If the node is part of another scene, remove it.
    if (node->_scene && node->_scene != this)
    {
        node->_scene->removeNode(node);
    }

    // If the node is part of another node hierarchy, remove it.
    if (node->getParent())
    {
        node->getParent()->removeChild(node);
    }

    // Link the new node into our list.
    if (_lastNode)
    {
        _lastNode->_nextSibling = node;
        node->_prevSibling = _lastNode;
        _lastNode = node;
    }
    else
    {
        _firstNode = _lastNode = node;
    }

    node->_scene = this;

    ++_nodeCount;

    // If we don't have an active camera set, then check for one and set it.
    if (_activeCamera == NULL)
    {
        Camera* camera = node->getCamera();
        if (camera)
        {
            setActiveCamera(camera);
        }
    }
}

void Scene::removeNode(Node* node)
{
    GP_ASSERT(node);

    if (node->_scene != this)
        return;

    if (node == _firstNode)
    {
        _firstNode = node->_nextSibling;
    }
    if (node == _lastNode)
    {
        _lastNode = node->_prevSibling;
    }

    node->remove();
    node->_scene = NULL;

    SAFE_RELEASE(node);

    --_nodeCount;
}

void Scene::removeAllNodes()
{
    while (_lastNode)
    {
        removeNode(_lastNode);
    }
}

unsigned int Scene::getNodeCount() const
{
    return _nodeCount;
}

Node* Scene::getFirstNode() const
{
    return _firstNode;
}

Camera* Scene::getActiveCamera() const
{
    return _activeCamera;
}

void Scene::setActiveCamera(Camera* camera)
{
    // Make sure we don't release the camera if the same camera is set twice.
    if (_activeCamera != camera)
    {
        AudioListener* audioListener = AudioListener::getInstance();

        if (_activeCamera)
        {
            // Unbind the active camera from the audio listener
            if (audioListener && (audioListener->getCamera() == _activeCamera))
            {
                audioListener->setCamera(NULL);
            }

            SAFE_RELEASE(_activeCamera);
        }

        _activeCamera = camera;

        if (_activeCamera)
        {
            _activeCamera->addRef();

            if (audioListener && _bindAudioListenerToCamera)
            {
                audioListener->setCamera(_activeCamera);
            }
        }
    }
}

void Scene::bindAudioListenerToCamera(bool bind)
{
    if (_bindAudioListenerToCamera != bind)
    {
        _bindAudioListenerToCamera = bind;

        if (AudioListener::getInstance())
        {
            AudioListener::getInstance()->setCamera(bind ? _activeCamera : NULL);
        }
    }
}

const Vector3& Scene::getAmbientColor() const
{
    return _ambientColor;
}

void Scene::setAmbientColor(float red, float green, float blue)
{
    _ambientColor.set(red, green, blue);
}

const Vector3& Scene::getLightColor() const
{
    return _lightColor;
}

void Scene::setLightColor(float red, float green, float blue)
{
    _lightColor.set(red, green, blue);
}

const Vector3& Scene::getLightDirection() const
{
    return _lightDirection;
}

void Scene::setLightDirection(const Vector3& direction)
{
    _lightDirection = direction;
}

static Material* createDebugMaterial()
{
    // Vertex shader for drawing colored lines.
    const char* vs_str = 
    {
        "uniform mat4 u_viewProjectionMatrix;\n"
        "attribute vec4 a_position;\n"
        "attribute vec4 a_color;\n"
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "    v_color = a_color;\n"
        "    gl_Position = u_viewProjectionMatrix * a_position;\n"
        "}"
    };

    // Fragment shader for drawing colored lines.
    const char* fs_str = 
    {
    #ifdef OPENGL_ES
        "precision highp float;\n"
    #endif
        "varying vec4 v_color;\n"
        "void main(void) {\n"
        "   gl_FragColor = v_color;\n"
        "}"
    };

    Effect* effect = Effect::createFromSource(vs_str, fs_str);
    Material* material = Material::create(effect);
    GP_ASSERT(material && material->getStateBlock());
    material->getStateBlock()->setDepthTest(true);

    SAFE_RELEASE(effect);

    return material;
}

/**
 * DebugVertex structure.
 * @script{ignore}
 */
struct DebugVertex
{
    /**
     * The x coordinate of the vertex.
     */
    float x;

    /**
     * The y coordinate of the vertex.
     */
    float y;
    
    /**
     * The z coordinate of the vertex.
     */
    float z;
    
    /** 
     * The red color component of the vertex.
     */
    float r;

    /** 
     * The green color component of the vertex.
     */
    float g;
    
    /** 
     * The blue color component of the vertex.
     */
    float b;
    
    /** 
     * The alpha component of the vertex.
     */
    float a;
};

static void drawDebugLine(MeshBatch* batch, const Vector3& point1, const Vector3& point2, const Vector3& color)
{
    GP_ASSERT(batch);

    static DebugVertex verts[2];

    verts[0].x = point1.x;
    verts[0].y = point1.y;
    verts[0].z = point1.z;
    verts[0].r = color.x;
    verts[0].g = color.y;
    verts[0].b = color.z;
    verts[0].a = 1.0f;

    verts[1].x = point2.x;
    verts[1].y = point2.y;
    verts[1].z = point2.z;
    verts[1].r = color.x;
    verts[1].g = color.y;
    verts[1].b = color.z;
    verts[1].a = 1.0f;

    batch->add(verts, 2);
}

#define DEBUG_BOX_COLOR Vector3(0, 1, 0)
#define DEBUG_SPHERE_COLOR Vector3(0, 1, 0)

static void drawDebugBox(MeshBatch* batch, const BoundingBox& box, const Matrix& matrix)
{
    if (box.isEmpty())
        return;

    // Transform box into world space (since we only store local boxes on mesh)
    BoundingBox worldSpaceBox(box);
    worldSpaceBox.transform(matrix);

    // Get box corners
    static Vector3 corners[8];
    worldSpaceBox.getCorners(corners);

    // Draw box lines
    drawDebugLine(batch, corners[0], corners[1], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[1], corners[2], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[2], corners[3], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[3], corners[0], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[4], corners[5], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[5], corners[6], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[6], corners[7], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[7], corners[4], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[0], corners[7], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[1], corners[6], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[2], corners[5], DEBUG_BOX_COLOR);
    drawDebugLine(batch, corners[3], corners[4], DEBUG_BOX_COLOR);
}

static void drawDebugSphere(MeshBatch* batch, const BoundingSphere& sphere)
{
    if (sphere.isEmpty())
        return;

    // Draw three rings for the sphere (one for the x, y and z axes)
    Vector3 pos1, pos2;
    float step = MATH_PI * 0.2f;
    float max = MATH_PIX2 + step;

    // X ring
    for (float r = 0.0f; r < max; r += step)
    {
        pos2.x = sphere.center.x;
        pos2.y = sphere.center.y + std::cos(r) * sphere.radius;
        pos2.z = sphere.center.z + std::sin(r) * sphere.radius;

        if (r > 0)
            drawDebugLine(batch, pos1, pos2, DEBUG_SPHERE_COLOR);

        pos1 = pos2;
    }

    // Y ring
    for (float r = 0.0f; r < max; r += step)
    {
        pos2.x = sphere.center.x + std::cos(r) * sphere.radius;
        pos2.y = sphere.center.y;
        pos2.z = sphere.center.z + std::sin(r) * sphere.radius;

        if (r > 0)
            drawDebugLine(batch, pos1, pos2, DEBUG_SPHERE_COLOR);

        pos1 = pos2;
    }

    // Z ring
    for (float r = 0.0f; r < max; r += step)
    {
        pos2.x = sphere.center.x + std::cos(r) * sphere.radius;
        pos2.y = sphere.center.y + std::sin(r) * sphere.radius;
        pos2.z = sphere.center.z;

        if (r > 0)
            drawDebugLine(batch, pos1, pos2, DEBUG_SPHERE_COLOR);

        pos1 = pos2;
    }
}

static void drawDebugNode(Scene* scene, MeshBatch* batch, Node* node, unsigned int debugFlags)
{
    GP_ASSERT(node);

    // If the node isn't visible, don't draw its bounds
    Camera* camera = scene->getActiveCamera();
    if (camera)
    {
        const BoundingSphere& sphere = node->getBoundingSphere();
        if (!sphere.isEmpty() && !camera->getFrustum().intersects(sphere))
            return;
    }

    if (debugFlags & Scene::DEBUG_BOXES)
    {
        if (node->getModel())
        {
            Model* model = node->getModel();
            GP_ASSERT(model->getMesh());

            MeshSkin* skin = model->getSkin();
            if (skin && skin->getRootJoint() && skin->getRootJoint()->getParent())
            {
                // For skinned meshes that have a parent node to the skin's root joint,
                // we need to transform the bounding volume by that parent node's transform
                // as well to get the full skinned bounding volume.
                drawDebugBox(batch, model->getMesh()->getBoundingBox(), node->getWorldMatrix() * skin->getRootJoint()->getParent()->getWorldMatrix());
            }
            else
            {
                drawDebugBox(batch, model->getMesh()->getBoundingBox(), node->getWorldMatrix());
            }
        }

        if (node->getTerrain())
        {
            drawDebugBox(batch, node->getTerrain()->getBoundingBox(), node->getWorldMatrix());
        }
    }

    if (debugFlags & Scene::DEBUG_SPHERES)
    {
        drawDebugSphere(batch, node->getBoundingSphere());
    }

    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        drawDebugNode(scene, batch, child, debugFlags);
    }
}

void Scene::drawDebug(unsigned int debugFlags)
{
    if (_debugBatch == NULL)
    {
        Material* material = createDebugMaterial();

        VertexFormat::Element elements[] =
        {
            VertexFormat::Element(VertexFormat::POSITION, 3),
            VertexFormat::Element(VertexFormat::COLOR, 4)
        };

        _debugBatch = MeshBatch::create(VertexFormat(elements, 2), Mesh::LINES, material, false);

        SAFE_RELEASE(material);
    }

    _debugBatch->start();

    for (Node* node = _firstNode; node != NULL; node = node->_nextSibling)
    {
        drawDebugNode(this, _debugBatch, node, debugFlags);
    }

    _debugBatch->finish();

    if (_activeCamera)
    {
        GP_ASSERT(_debugBatch->getMaterial());
        GP_ASSERT(_debugBatch->getMaterial()->getParameter("u_viewProjectionMatrix"));
        _debugBatch->getMaterial()->getParameter("u_viewProjectionMatrix")->setValue(_activeCamera->getViewProjectionMatrix());
    }

    _debugBatch->draw();
}

}
