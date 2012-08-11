#include "Base.h"
#include "AudioListener.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "MeshSkin.h"
#include "Joint.h"

namespace gameplay
{

Scene::Scene() : _activeCamera(NULL), _firstNode(NULL), _lastNode(NULL), _nodeCount(0), _bindAudioListenerToCamera(true), _debugBatch(NULL)
{
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
}

Scene* Scene::create()
{
    return new Scene();
}

Scene* Scene::load(const char* filePath)
{
    return SceneLoader::load(filePath);
}

const char* Scene::getId() const
{
    return _id.c_str();
}

void Scene::setId(const char* id)
{
    if (id)
    {
        _id = id;
    }
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

static void drawDebugNode(MeshBatch* batch, Node* node, unsigned int debugFlags)
{
    GP_ASSERT(node);
    Model* model = node->getModel();

    if ((debugFlags & Scene::DEBUG_BOXES) && model)
    {
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

    if ((debugFlags & Scene::DEBUG_SPHERES) && model)
    {
        drawDebugSphere(batch, node->getBoundingSphere());
    }

    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        drawDebugNode(batch, child, debugFlags);
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
        drawDebugNode(_debugBatch, node, debugFlags);
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
