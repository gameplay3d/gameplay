#include "Base.h"
#include "Camera.h"

#define CAMERA_DIRTY_VIEW 1
#define CAMERA_DIRTY_PROJ 2
#define CAMERA_DIRTY_VIEW_PROJ 4
#define CAMERA_DIRTY_INV_VIEW 8
#define CAMERA_DIRTY_INV_VIEW_PROJ 16
#define CAMERA_DIRTY_BOUNDS 32
#define CAMERA_DIRTY_ALL (CAMERA_DIRTY_VIEW | CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS)
#define CAMERA_CUSTOM_PROJECTION 64
#define CAMERA_FIELD_OF_VIEW 60.0f
#define CAMERA_ZOOM 1.0f
#define CAMERA_ASPECT_RATIO GP_GRAPHICS_WIDTH / GP_GRAPHICS_HEIGHT
#define CAMERA_NEAR_PLANE 0.1f
#define CAMERA_FAR_PLANE 1000.f

namespace gameplay
{

Camera::Camera() : Component(),
    _mode(MODE_PERSPECTIVE),
    _fieldOfView(CAMERA_FIELD_OF_VIEW),
    _aspectRatio(CAMERA_ASPECT_RATIO),
    _nearPlane(CAMERA_NEAR_PLANE),
    _farPlane(CAMERA_FAR_PLANE),
    _zoom(CAMERA_ZOOM),
    _dirtyBits(CAMERA_DIRTY_ALL)
{
}

Camera::~Camera()
{
}

Camera::Mode Camera::getMode() const
{
    return _mode;
}

void Camera::setMode(Camera::Mode mode)
{
    _mode = mode;
    _dirtyBits = CAMERA_DIRTY_ALL;
}

float Camera::getFieldOfView() const
{
    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
    _fieldOfView = fieldOfView;
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

float Camera::getZoom() const
{
    return _zoom;
}

void Camera::setZoom(float zoom)
{
    _zoom = zoom;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

const Matrix& Camera::getViewMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_VIEW)
    {
        if (_object)
        {
            // The view matrix is the inverse of our transform matrix.
            _object->getWorldToLocalMatrix().invert(&_viewMatrix);
        }
        else
        {
            _viewMatrix.setIdentity();
        }

        _dirtyBits &= ~CAMERA_DIRTY_VIEW;
    }
    return _viewMatrix;
}

const Matrix& Camera::getInverseViewMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_INV_VIEW)
    {
        getViewMatrix().invert(&_inverseViewMatrix);

        _dirtyBits &= ~CAMERA_DIRTY_INV_VIEW;
    }
    return _inverseViewMatrix;
}

const Matrix& Camera::getProjectionMatrix() const
{
    if (!(_dirtyBits & CAMERA_CUSTOM_PROJECTION) && (_dirtyBits & CAMERA_DIRTY_PROJ))
    {
        if (_mode == MODE_PERSPECTIVE)
        {
            Matrix::createPerspective(_fieldOfView, _aspectRatio, _nearPlane, _farPlane, &_projectionMatrix);
        }
        else
        {
            // Create an ortho projection with the origin at the bottom left of the viewport, +X to the right and +Y up.
            Matrix::createOrthographic(_zoom, _zoom, _nearPlane, _farPlane, &_projectionMatrix);
        }

        _dirtyBits &= ~CAMERA_DIRTY_PROJ;
    }
    return _projectionMatrix;
}

void Camera::setProjectionMatrix(const Matrix& matrix)
{
    _projectionMatrix = matrix;
    _dirtyBits |= CAMERA_CUSTOM_PROJECTION;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

const Matrix& Camera::getViewProjectionMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_VIEW_PROJ)
    {
        Matrix::multiply(getProjectionMatrix(), getViewMatrix(), &_viewProjectionMatrix);
        _dirtyBits &= ~CAMERA_DIRTY_VIEW_PROJ;
    }
    return _viewProjectionMatrix;
}

const Matrix& Camera::getInverseViewProjectionMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_INV_VIEW_PROJ)
    {
        getViewProjectionMatrix().invert(&_inverseViewProjectionMatrix);
        _dirtyBits &= ~CAMERA_DIRTY_INV_VIEW_PROJ;
    }
    return _inverseViewProjectionMatrix;
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
        *depth = (ndcZ + 1.0f) / 2.0f;
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

std::string Camera::getClassName()
{
	return "gameplay::Camera";
}

void Camera::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("mode", "gameplay::Camera::Mode", _mode, -1);
    if (_mode == Camera::MODE_PERSPECTIVE)
        serializer->writeFloat("fieldOfView", _fieldOfView, CAMERA_FIELD_OF_VIEW);
    else
        serializer->writeFloat("zoom", _zoom, CAMERA_ZOOM);
    serializer->writeFloat("aspectRatio", _aspectRatio, CAMERA_ASPECT_RATIO);
    serializer->writeFloat("nearPlane", _nearPlane, CAMERA_NEAR_PLANE);
    serializer->writeFloat("farPlane", _farPlane, CAMERA_FAR_PLANE);
}

void Camera::onDeserialize(Serializer* serializer)
{
    _mode = static_cast<Camera::Mode>(serializer->readEnum("mode", "gameplay::Camera::Mode", -1));
    if (_mode == Camera::MODE_PERSPECTIVE)
        _fieldOfView = serializer->readFloat("fieldOfView", CAMERA_FIELD_OF_VIEW);
    else
        _zoom = serializer->readFloat("zoom", CAMERA_ZOOM);
    _aspectRatio = serializer->readFloat("aspectRatio", CAMERA_ASPECT_RATIO);
    _nearPlane = serializer->readFloat("nearPlane", CAMERA_NEAR_PLANE);
    _farPlane = serializer->readFloat("farPlane", CAMERA_FAR_PLANE);
}

std::shared_ptr<Serializable> Camera::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Camera>());
}

std::string Camera::enumToString(const std::string& enumName, int value)
{
    if (enumName.compare("gameplay::Camera::Mode") == 0)
    {
        switch (value)
        {
            case Camera::MODE_PERSPECTIVE:
                return "MODE_PERSPECTIVE";
            case Camera::MODE_ORTHOGRAPHIC:
                return "MODE_ORTHOGRAPHIC";
            default:
                break;
        }
    }
    return "MODE_PERSPECTIVE";
}

int Camera::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Camera::Mode") == 0)
    {
        if (str.compare("MODE_PERSPECTIVE") == 0)
            return Camera::MODE_PERSPECTIVE;
        else if (str.compare("MODE_ORTHOGRAPHIC") == 0)
            return Camera::MODE_ORTHOGRAPHIC;
    }
    return Camera::MODE_PERSPECTIVE;
}

void Camera::setObject(std::shared_ptr<SceneObject> object)
{
    Component::setObject(object);
    _dirtyBits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

}
