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

namespace gameplay
{

Camera::Camera(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
    : _type(PERSPECTIVE), _fieldOfView(fieldOfView), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane),
      _dirtyBits(CAMERA_DIRTY_ALL), _node(NULL)
{
}

Camera::Camera(float zoomX, float zoomY, float aspectRatio, float nearPlane, float farPlane)
    : _type(ORTHOGRAPHIC), _aspectRatio(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane),
      _dirtyBits(CAMERA_DIRTY_ALL), _node(NULL)
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

Camera::Type Camera::getCameraType() const
{
    return _type;
}

float Camera::getFieldOfView() const
{
    assert(_type == Camera::PERSPECTIVE);

    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
    assert(_type == Camera::PERSPECTIVE);

    _fieldOfView = fieldOfView;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getZoomX() const
{
    assert(_type == Camera::ORTHOGRAPHIC);

    return _zoom[0];
}

void Camera::setZoomX(float zoomX)
{
    assert(_type == Camera::ORTHOGRAPHIC);

    _zoom[0] = zoomX;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getZoomY() const
{
    assert(_type == Camera::ORTHOGRAPHIC);

    return _zoom[1];
}

void Camera::setZoomY(float zoomY)
{
    assert(_type == Camera::ORTHOGRAPHIC);

    _zoom[1] = zoomY;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getAspectRatio() const
{
    return _aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getNearPlane() const
{
    return _nearPlane;
}

void Camera::setNearPlane(float nearPlane)
{
    _nearPlane = nearPlane;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getFarPlane() const
{
    return _farPlane;
}

void Camera::setFarPlane(float farPlane)
{
    _farPlane = farPlane;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
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

        _dirtyBits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
    }
}

const Matrix& Camera::getViewMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_VIEW)
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

        _dirtyBits &= ~CAMERA_DIRTY_VIEW;
    }

    return _view;
}

const Matrix& Camera::getInverseViewMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_INV_VIEW)
    {
        getViewMatrix().invert(&_inverseView);

        _dirtyBits &= ~CAMERA_DIRTY_INV_VIEW;
    }

    return _inverseView;
}

const Matrix& Camera::getProjectionMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_PROJ)
    {
        if (_type == PERSPECTIVE)
        {
            Matrix::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &_projection);
        }
        else
        {
            Matrix::createOrthographic(_zoom[0], _zoom[1], _nearPlane, _farPlane, &_projection);
        }

        _dirtyBits &= ~CAMERA_DIRTY_PROJ;
    }

    return _projection;
}

const Matrix& Camera::getViewProjectionMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_VIEW_PROJ)
    {
        Matrix::multiply(getProjectionMatrix(), getViewMatrix(), &_viewProjection);

        _dirtyBits &= ~CAMERA_DIRTY_VIEW_PROJ;
    }

    return _viewProjection;
}

const Matrix& Camera::getInverseViewProjectionMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_INV_VIEW_PROJ)
    {
        getViewProjectionMatrix().invert(&_inverseViewProjection);

        _dirtyBits &= ~CAMERA_DIRTY_INV_VIEW_PROJ;
    }

    return _inverseViewProjection;
}

const Frustum& Camera::getFrustum() const
{
    if (_dirtyBits & CAMERA_DIRTY_BOUNDS)
    {
        // Update our bounding frustum from our view projection matrix.
        _bounds.set(getViewProjectionMatrix());

        _dirtyBits &= ~CAMERA_DIRTY_BOUNDS;
    }

    return _bounds;
}

void Camera::project(const Viewport* viewport, const Vector3& position, float* x, float* y, float* depth)
{
    // Determine viewport coords to use.
    float vpx, vpy, vpw, vph;
    if (viewport)
    {
        vpx = viewport->getX();
        vpy = viewport->getY();
        vpw = viewport->getWidth();
        vph = viewport->getHeight();
    }
    else
    {
        vpx = 0;
        vpy = 0;
        vpw = Game::getInstance()->getWidth();
        vph = Game::getInstance()->getHeight();
    }

    // Transform the point to clip-space.
    Vector4 clipPos;
    getViewProjectionMatrix().transformVector(Vector4(position.x, position.y, position.z, 1.0f), &clipPos);

    // Compute normalized device coordinates.
    float ndcX = clipPos.x / clipPos.w;
    float ndcY = clipPos.y / clipPos.w;

    // Compute screen coordinates by applying our viewport transformation.
    *x = vpx + (ndcX + 1.0f) * 0.5f * vpw;
    *y = vpy + (1.0f - (ndcY + 1.0f) * 0.5f) * vph;
    if (depth)
    {
        float ndcZ = clipPos.z / clipPos.w;
        *depth = ndcZ + 1.0f / 2.0f;
    }
}

void Camera::unproject(const Viewport* viewport, float x, float y, float depth, Vector3* dst)
{
    // Determine viewport coords to use.
    float vpx, vpy, vpw, vph;
    if (viewport)
    {
        vpx = viewport->getX();
        vpy = viewport->getY();
        vpw = viewport->getWidth();
        vph = viewport->getHeight();
    }
    else
    {
        vpx = 0;
        vpy = 0;
        vpw = Game::getInstance()->getWidth();
        vph = Game::getInstance()->getHeight();
    }
    
    // Create our screen space position in NDC.
    Vector4 screen(
        ((float)x - (float)vpx) / (float)vpw,
        ((float)(vph - y) - (float)vpy) / (float)vph,
        depth,
        1.0f );

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

void Camera::pickRay(const Viewport* viewport, float x, float y, Ray* dst)
{
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
    assert(cameraClone);

    if (Node* node = context.findClonedNode(getNode()))
    {
        cameraClone->setNode(node);
    }
    return cameraClone;
}

void Camera::transformChanged(Transform* transform, long cookie)
{
    _dirtyBits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

}
