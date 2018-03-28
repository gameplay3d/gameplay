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
#define CAMERA_SIZE 5.0f
#define CAMERA_CLIP_PLANE_NEAR 0.1f
#define CAMERA_CLIP_PLANE_FAR 1000.f

namespace gameplay
{

Camera::Camera() : Component(),
    _mode(Mode::ePerspective),
    _fieldOfView(CAMERA_FIELD_OF_VIEW),
    _size(CAMERA_SIZE),
    _clipPlaneNear(CAMERA_CLIP_PLANE_NEAR),
    _clipPlaneFar(CAMERA_CLIP_PLANE_FAR),
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

float Camera::getSize() const
{
    return _size;
}

void Camera::setSize(float size)
{
    _size = size;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}


float Camera::getClipPlaneNear() const
{
    return _clipPlaneNear;
}

void Camera::setClipPlaneNear(float clipPlaneNear)
{
    _clipPlaneNear = clipPlaneNear;
    _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

float Camera::getClipPlaneFar() const
{
    return _clipPlaneFar;
}

void Camera::setClipPlaneFar(float clipPlaneFar)
{
    _clipPlaneFar = clipPlaneFar;
   _dirtyBits |= CAMERA_DIRTY_PROJ | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

void Camera::reset(Camera::Mode mode)
{
    // TODO
}

const Matrix& Camera::getViewMatrix() const
{
    if (_dirtyBits & CAMERA_DIRTY_VIEW)
    {
        if (_object.lock())
        {
            // The view matrix is the inverse of our transform matrix.
            _object.lock()->getWorldToLocalMatrix().invert(&_viewMatrix);
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
        if (_mode == Mode::ePerspective)
        {
            Matrix::createPerspective(_fieldOfView, _aspectRatio, _clipPlaneNear, _clipPlaneFar, &_projectionMatrix);
        }
        else
        {
            // Create an ortho projection with the origin at the bottom left of the viewport, +X to the right and +Y up.
            Matrix::createOrthographic(_size, _size, _clipPlaneNear, _clipPlaneFar, &_projectionMatrix);
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

Component::ClassType Camera::getClassType()
{
    return ClassType::eCamera;
}

std::string Camera::getClassName()
{
    return "gameplay::Camera";
}

void Camera::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("mode", "gameplay::Camera::Mode", static_cast<int>(_mode), -1);
    if (_mode == Mode::ePerspective)
    {
        serializer->writeFloat("fieldOfView", _fieldOfView, CAMERA_FIELD_OF_VIEW);
    }
    else
    {
        serializer->writeFloat("size", _size, CAMERA_SIZE);
    }
    serializer->writeFloat("clipPlaneNear", _clipPlaneNear, CAMERA_CLIP_PLANE_NEAR);
    serializer->writeFloat("clipPlaneFar", _clipPlaneFar, CAMERA_CLIP_PLANE_FAR);


}

void Camera::onDeserialize(Serializer* serializer)
{
    _mode = static_cast<Camera::Mode>(serializer->readEnum("mode", "gameplay::Camera::Mode", -1));
    if (_mode == Mode::ePerspective)
    {
        _fieldOfView = serializer->readFloat("fieldOfView", CAMERA_FIELD_OF_VIEW);
    }
    else
    {
        _size = serializer->readFloat("size", CAMERA_SIZE);
    }
    _clipPlaneNear = serializer->readFloat("clipPlaneNear", CAMERA_CLIP_PLANE_NEAR);
    _clipPlaneFar = serializer->readFloat("clipPlaneFar", CAMERA_CLIP_PLANE_FAR);
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
            case static_cast<int>(Mode::ePerspective):
                return "ePerspective";
            case static_cast<int>(Mode::eOrthograhic):
                return "eOrthograhic";
            default:
                break;
        }
    }
    return "";
}

int Camera::enumParse(const std::string& enumName, const std::string& str)
{
    if (enumName.compare("gameplay::Camera::Mode") == 0)
    {
        if (str.compare("ePerspective") == 0)
            return static_cast<int>(Mode::ePerspective);
        else if (str.compare("eOrthograhic") == 0)
            return static_cast<int>(Mode::eOrthograhic);
    }
    return static_cast<int>(Mode::ePerspective);
}

void Camera::setObject(std::shared_ptr<SceneObject> object)
{
    Component::setObject(object);
    _dirtyBits |= CAMERA_DIRTY_VIEW | CAMERA_DIRTY_VIEW_PROJ | CAMERA_DIRTY_INV_VIEW | CAMERA_DIRTY_INV_VIEW_PROJ | CAMERA_DIRTY_BOUNDS;
}

}
