#include "Base.h"
#include "Camera.h"
#include "Game.h"
#include "Node.h"
#include "Game.h"
#include "PhysicsController.h"

// Camera dirty bits
#define CAMERA_DIRTY_VIEW 1
#define CAMERA_DIRTY_PROJ 2
#define CAMERA_DIRTY_VIEW_PROJ 4
#define CAMERA_DIRTY_INV_VIEW 8
#define CAMERA_DIRTY_INV_VIEW_PROJ 16
#define CAMERA_DIRTY_BOUNDS 32
#define CAMERA_DIRTY_ALL (CAMERA_DIRTY_VIEW | CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS)

// Other misc camera bits
#define CAMERA_CUSTOM_PROJECTION 64

namespace gameplay
{

Camera::Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
    : _type(PERSPECTIVE), _fieldOfView(fieldOfView), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane),
      _bits(CAMERA_DIRTY_ALL), _node(NULL)
{
}

Camera::Camera(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
    : _type(ORTHOGRAPHIC), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane),
      _bits(CAMERA_DIRTY_ALL), _node(NULL)
{
    // Orthographic camera.
    _zoom[0] = zoomX;
    _zoom[1] = zoomY;
}

Camera::~Camera()
{
}

Camera* Camera::createPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
    return new Camera(fieldOfView, aspectRatio, nearPlane, farPlane);
}

Camera* Camera::createOrthographic(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
{
    return new Camera(zoomX, zoomY, aspectRatio, nearPlane, farPlane);
}

Camera* Camera::create(Properties* properties)
{
    GP_ASSERT(properties);

    // Read camera type
    std::string typeStr;
    if (properties->exists("type"))
        typeStr = properties->getString("type");
    Camera::Type type;
    if (typeStr == "PERSPECTIVE")
    {
        type = Camera::PERSPECTIVE;
    }
    else if (typeStr == "ORTHOGRAPHIC")
    {
        type = Camera::ORTHOGRAPHIC;
    }
    else
    {
        GP_ERROR("Invalid 'type' parameter for camera definition.");
        return NULL;
    }

    // Read common parameters
    float aspectRatio, nearPlane, farPlane;
    if (properties->exists("aspectRatio"))
    {
        aspectRatio = properties->getFloat("aspectRatio");
    }
    else
    {
        // Use default aspect ratio
        aspectRatio = (float)Game::getInstance()->getWidth() / Game::getInstance()->getHeight();
    }

    if (properties->exists("nearPlane"))
        nearPlane = properties->getFloat("nearPlane");
    else
        nearPlane = 0.2f; // use some reasonable default value

    if (properties->exists("farPlane"))
        farPlane = properties->getFloat("farPlane");
    else
        farPlane = 100; // use some reasonable default value

    Camera* camera = NULL;

    switch (type)
    {
    case Camera::PERSPECTIVE:
        // If field of view is not specified, use a default of 60 degrees
        camera = createPerspective(
            properties->exists("fieldOfView") ? properties->getFloat("fieldOfView") : 60.0f,
            aspectRatio, nearPlane, farPlane);
        break;

    case Camera::ORTHOGRAPHIC:
        // If zoomX and zoomY are not specified, use screen width/height
        camera = createOrthographic(
            properties->exists("zoomX") ? properties->getFloat("zoomX") : Game::getInstance()->getWidth(),
            properties->exists("zoomY") ? properties->getFloat("zoomY") : Game::getInstance()->getHeight(),
            aspectRatio, nearPlane, farPlane);
        break;
    }

    return camera;
}

Camera::Type Camera::getCameraType() const
{
    return _type;
}

float Camera::getFieldOfView() const
{
    GP_ASSERT(_type == Camera::PERSPECTIVE);

    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
    GP_ASSERT(_type == Camera::PERSPECTIVE);

    _fieldOfView = fieldOfView;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getZoomX() const
{
    GP_ASSERT(_type == Camera::ORTHOGRAPHIC);

    return _zoom[0];
}

void Camera::setZoomX(float zoomX)
{
    GP_ASSERT(_type == Camera::ORTHOGRAPHIC);

    _zoom[0] = zoomX;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getZoomY() const
{
    GP_ASSERT(_type == Camera::ORTHOGRAPHIC);

    return _zoom[1];
}

void Camera::setZoomY(float zoomY)
{
    GP_ASSERT(_type == Camera::ORTHOGRAPHIC);

    _zoom[1] = zoomY;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getNearPlane() const
{
    return _nearPlane;
}

void Camera::setNearPlane(float nearPlane)
{
    _nearPlane = nearPlane;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getFarPlane() const
{
    return _farPlane;
}

void Camera::setFarPlane(float farPlane)
{
    _farPlane = farPlane;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

Node* Camera::getNode() const
{
    return _node;
}

void Camera::setNode(Node* node)
{
    if (_node != node)
    {
        if (_node)
        {
            _node->removeListener(this);
        }

        // Connect the new node.
        _node = node;

        if (_node)
        {
            _node->addListener(this);
        }

        _bits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
    }
}

const Matrix& Camera::getViewMatrix() const
{
    if (_bits & CAMERA_DIRTY_VIEW)
    {
        if (_node)
        {
            // The view matrix is the inverse of our transform matrix.
            _node->getWorldMatrix().invert(&_view);
        }
        else
        {
            _view.setIdentity();
        }

        _bits &= ~CAMERA_DIRTY_VIEW;
    }

    return _view;
}

const Matrix& Camera::getInverseViewMatrix() const
{
    if (_bits & CAMERA_DIRTY_INV_VIEW)
    {
        getViewMatrix().invert(&_inverseView);

        _bits &= ~CAMERA_DIRTY_INV_VIEW;
    }

    return _inverseView;
}

const Matrix& Camera::getProjectionMatrix() const
{
    if (!(_bits & CAMERA_CUSTOM_PROJECTION) && (_bits & CAMERA_DIRTY_PROJ))
    {
        if (_type == PERSPECTIVE)
        {
            Matrix::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &_projection);
        }
        else
        {
            Matrix::createOrthographic(_zoom[0], _zoom[1], _nearPlane, _farPlane, &_projection);
        }

        _bits &= ~CAMERA_DIRTY_PROJ;
    }

    return _projection;
}

void Camera::setProjectionMatrix(const Matrix& matrix)
{
    _projection = matrix;
    _bits |= CAMERA_CUSTOM_PROJECTION;
    _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

void Camera::resetProjectionMatrix()
{
    if (_bits & CAMERA_CUSTOM_PROJECTION)
    {
        _bits &= ~CAMERA_CUSTOM_PROJECTION;
        _bits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
    }
}

const Matrix& Camera::getViewProjectionMatrix() const
{
    if (_bits & CAMERA_DIRTY_VIEW_PROJ)
    {
        Matrix::multiply(getProjectionMatrix(), getViewMatrix(), &_viewProjection);

        _bits &= ~CAMERA_DIRTY_VIEW_PROJ;
    }

    return _viewProjection;
}

const Matrix& Camera::getInverseViewProjectionMatrix() const
{
    if (_bits & CAMERA_DIRTY_INV_VIEW_PROJ)
    {
        getViewProjectionMatrix().invert(&_inverseViewProjection);

        _bits &= ~CAMERA_DIRTY_INV_VIEW_PROJ;
    }

    return _inverseViewProjection;
}

const Frustum& Camera::getFrustum() const
{
    if (_bits & CAMERA_DIRTY_BOUNDS)
    {
        // Update our bounding frustum from our view projection matrix.
        _bounds.set(getViewProjectionMatrix());

        _bits &= ~CAMERA_DIRTY_BOUNDS;
    }

    return _bounds;
}

void Camera::project(const Rectangle& viewport, const Vector3& position, float* x, float* y, float* depth) const
{
    GP_ASSERT(x);
    GP_ASSERT(y);

    // Transform the point to clip-space.
    Vector4 clipPos;
    getViewProjectionMatrix().transformVector(Vector4(position.x, position.y, position.z, 1.0f), &clipPos);

    // Compute normalized device coordinates.
    GP_ASSERT(clipPos.w != 0.0f);
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;

    // Compute screen coordinates by applying our viewport transformation.
    *x = viewport.x + (ndcX + 1.0f) * 0.5f * viewport.width;
    *y = viewport.y + (1.0f - (ndcY + 1.0f) * 0.5f) * viewport.height;
    if (depth)
    {
        float ndcZ = clipPos.z / clipPos.w;
        *depth = ndcZ + 1.0f / 2.0f;
    }
}

void Camera::project(const Rectangle& viewport, const Vector3& position, Vector2* out) const
{
    GP_ASSERT(out);
    float x, y;
    project(viewport, position, &x, &y);
    out->set(x, y);
}

void Camera::project(const Rectangle& viewport, const Vector3& position, Vector3* out) const
{
    GP_ASSERT(out);
    float x, y, depth;
    project(viewport, position, &x, &y, &depth);
    out->set(x, y, depth);
}

void Camera::unproject(const Rectangle& viewport, float x, float y, float depth, Vector3* dst) const
{
    GP_ASSERT(dst);
    
    // Create our screen space position in NDC.
    GP_ASSERT(viewport.width != 0.0f && viewport.height != 0.0f);
    Vector4 screen((x - viewport.x) / viewport.width, ((viewport.height - y) - viewport.y) / viewport.height, depth, 1.0f);

    // Map to range -1 to 1.
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;

    // Transform the screen-space NDC by our inverse view projection matrix.
    getInverseViewProjectionMatrix().transformVector(screen, &screen);

    // Divide by our W coordinate.
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }

    dst->set(screen.x, screen.y, screen.z);
}

void Camera::pickRay(const Rectangle& viewport, float x, float y, Ray* dst) const
{
    GP_ASSERT(dst);

    // Get the world-space position at the near clip plane.
    Vector3 nearPoint;
    unproject(viewport, x, y, 0.0f, &nearPoint);

    // Get the world-space position at the far clip plane.
    Vector3 farPoint;
    unproject(viewport, x, y, 1.0f, &farPoint);

    // Set the direction of the ray.
    Vector3 direction;
    Vector3::subtract(farPoint, nearPoint, &direction);
    direction.normalize();

    dst->set(nearPoint, direction);
}

Camera* Camera::clone(NodeCloneContext &context) const
{
    Camera* cameraClone = NULL;
    if (getCameraType() == PERSPECTIVE)
    {
        cameraClone = createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane);
    }
    else if (getCameraType() == ORTHOGRAPHIC)
    {
        cameraClone = createOrthographic(getZoomX(), getZoomY(), getAspectRatio(), _nearPlane, _farPlane);
    }
    GP_ASSERT(cameraClone);

    if (Node* node = context.findClonedNode(getNode()))
    {
        cameraClone->setNode(node);
    }
    return cameraClone;
}

void Camera::transformChanged(Transform* transform, long cookie)
{
    _bits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

}
