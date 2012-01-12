#include "Base.h"
#include "Transform.h"
#include "Game.h"

namespace gameplay
{

Transform::Transform()
    : _matrixDirty(false), _listeners(NULL)
{
    _targetType = AnimationTarget::TRANSFORM;
    _scale.set(Vector3::one());
}

Transform::Transform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
    : _matrixDirty(false), _listeners(NULL)
{
    _targetType = AnimationTarget::TRANSFORM;
    set(scale, rotation, translation);
}

Transform::Transform(const Vector3& scale, const Matrix& rotation, const Vector3& translation)
    : _matrixDirty(false), _listeners(NULL)
{
    _targetType = AnimationTarget::TRANSFORM;
    set(scale, rotation, translation);
}

Transform::Transform(const Transform& copy)
    : _matrixDirty(false), _listeners(NULL)
{
    _targetType = AnimationTarget::TRANSFORM;
    set(copy);
}

Transform::~Transform()
{
    SAFE_DELETE(_listeners);
}

const Matrix& Transform::getMatrix() const
{
    if (_matrixDirty)
    {
        _matrixDirty = false;

        bool hasTranslation = !_translation.isZero();
        bool hasScale = !_scale.isOne();
        bool hasRotation = !_rotation.isIdentity();

        // Compose the matrix in TRS order since we use column-major matrices with column vectors and
        // multiply M*v (as opposed to XNA and DirectX that use row-major matrices with row vectors and multiply v*M).
        if (hasTranslation)
        {
            Matrix::createTranslation(_translation, &_matrix);
            if (hasRotation)
            {
                _matrix.rotate(_rotation);
            }
            if (hasScale)
            {
                _matrix.scale(_scale);
            }
        }
        else if (hasRotation)
        {
            Matrix::createRotation(_rotation, &_matrix);
            if (hasScale)
            {
                _matrix.scale(_scale);
            }
        }
        else if (hasScale)
        {
            Matrix::createScale(_scale, &_matrix);
        }
    }

    return _matrix;
}

const Vector3& Transform::getScale() const
{
    return _scale;
}

void Transform::getScale(Vector3* scale) const
{
    scale->set(_scale);
}

float Transform::getScaleX() const
{
    return _scale.x;
}

float Transform::getScaleY() const
{
    return _scale.y;
}

float Transform::getScaleZ() const
{
    return _scale.z;
}

const Quaternion& Transform::getRotation() const
{
    return _rotation;
}

void Transform::getRotation(Quaternion* rotation) const
{
    assert(rotation);

    rotation->set(_rotation);
}

void Transform::getRotation(Matrix* rotation) const
{
    assert(rotation);

    Matrix::createRotation(_rotation, rotation);
}

float Transform::getRotation(Vector3* axis) const
{
    assert(axis);
    return _rotation.toAxisAngle(axis);
}

const Vector3& Transform::getTranslation() const
{
    return _translation;
}

void Transform::getTranslation(Vector3* translation) const
{
    translation->set(_translation);
}

float Transform::getTranslationX() const
{
    return _translation.x;
}

float Transform::getTranslationY() const
{
    return _translation.y;
}

float Transform::getTranslationZ() const
{
    return _translation.z;
}

Vector3 Transform::getForwardVector() const
{
    Vector3 v;
    getForwardVector(&v);
    return v;
}

void Transform::getForwardVector(Vector3* dst) const
{
    getMatrix().getForwardVector(dst);
}

Vector3 Transform::getBackVector() const
{
    Vector3 v;
    getBackVector(&v);
    return v;
}

void Transform::getBackVector(Vector3* dst) const
{
    getMatrix().getBackVector(dst);
}

Vector3 Transform::getUpVector() const
{
    Vector3 v;
    getUpVector(&v);
    return v;
}

void Transform::getUpVector(Vector3* dst) const
{
    getMatrix().getUpVector(dst);
}

Vector3 Transform::getDownVector() const
{
    Vector3 v;
    getDownVector(&v);
    return v;
}

void Transform::getDownVector(Vector3* dst) const
{
    getMatrix().getDownVector(dst);
}

Vector3 Transform::getLeftVector() const
{
    Vector3 v;
    getLeftVector(&v);
    return v;
}

void Transform::getLeftVector(Vector3* dst) const
{
    getMatrix().getLeftVector(dst);
}

Vector3 Transform::getRightVector() const
{
    Vector3 v;
    getRightVector(&v);
    return v;
}

void Transform::getRightVector(Vector3* dst) const
{
    getMatrix().getRightVector(dst);
}

void Transform::rotate(float qx, float qy, float qz, float qw)
{
    Quaternion q(qx, qy, qz, qw);
    _rotation.multiply(q);
    dirty();
}

void Transform::rotate(const Quaternion& rotation)
{
    _rotation.multiply(rotation);
    dirty();
}

void Transform::rotate(const Vector3& axis, float angle)
{
    Quaternion rotationQuat;
    Quaternion::createFromAxisAngle(axis, angle, &rotationQuat);
    _rotation.multiply(rotationQuat);
    _rotation.normalize();
    dirty();
}

void Transform::rotate(const Matrix& rotation)
{
    Quaternion rotationQuat;
    Quaternion::createFromRotationMatrix(rotation, &rotationQuat);
    _rotation.multiply(rotationQuat);
    dirty();
}

void Transform::rotateX(float angle)
{
    Quaternion rotationQuat;
    Quaternion::createFromAxisAngle(Vector3::unitX(), angle, &rotationQuat);
    _rotation.multiply(rotationQuat);
    dirty();
}

void Transform::rotateY(float angle)
{
    Quaternion rotationQuat;
    Quaternion::createFromAxisAngle(Vector3::unitY(), angle, &rotationQuat);
    _rotation.multiply(rotationQuat);
    dirty();
}

void Transform::rotateZ(float angle)
{
    Quaternion rotationQuat;
    Quaternion::createFromAxisAngle(Vector3::unitZ(), angle, &rotationQuat);
    _rotation.multiply(rotationQuat);
    dirty();
}

void Transform::scale(float scale)
{
    _scale.scale(scale);
    dirty();
}

void Transform::scale(float sx, float sy, float sz)
{
    _scale.x *= sx;
    _scale.y *= sy;
    _scale.z *= sz;
    dirty();
}

void Transform::scale(const Vector3& scale)
{
    _scale.x *= scale.x;
    _scale.y *= scale.y;
    _scale.z *= scale.z;
    dirty();
}

void Transform::scaleX(float sx)
{
    _scale.x *= sx;
    dirty();
}

void Transform::scaleY(float sy)
{
    _scale.y *= sy;
    dirty();
}

void Transform::scaleZ(float sz)
{
    _scale.z *= sz;
    dirty();
}

void Transform::set(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
{
    _scale.set(scale);
    _rotation.set(rotation);
    _translation.set(translation);
    dirty();
}

void Transform::set(const Vector3& scale, const Matrix& rotation, const Vector3& translation)
{
    _scale.set(scale);
    Quaternion rotationQuat;
    Quaternion::createFromRotationMatrix(rotation, &rotationQuat);
    _rotation.set(rotationQuat);
    _translation.set(translation);
    dirty();
}

void Transform::set(const Vector3& scale, const Vector3& axis, float angle, const Vector3& translation)
{
    _scale.set(scale);
    _rotation.set(axis, angle);
    _translation.set(translation);
}

void Transform::set(const Transform& transform)
{
    _scale.set(transform._scale);
    _rotation.set(transform._rotation);
    _translation.set(transform._translation);
    dirty();
}

void Transform::setScale(float scale)
{
    _scale.set(scale, scale, scale);
    dirty();
}

void Transform::setScale(float sx, float sy, float sz)
{
    _scale.set(sx, sy, sz);
    dirty();
}

void Transform::setScale(const Vector3& scale)
{
    _scale.set(scale);
    dirty();
}

void Transform::setScaleX(float sx)
{
    _scale.x = sx;
    dirty();
}

void Transform::setScaleY(float sy)
{
    _scale.y = sy;
    dirty();
}

void Transform::setScaleZ(float sz)
{
    _scale.z = sz;
    dirty();
}

void Transform::setRotation(const Quaternion& rotation)
{
    _rotation.set(rotation);
    dirty();
}

void Transform::setRotation(float qx, float qy, float qz, float qw)
{
    _rotation.set(qx, qy, qz, qw);
    dirty();
}

void Transform::setRotation(const Matrix& rotation)
{
    Quaternion rotationQuat;
    Quaternion::createFromRotationMatrix(rotation, &rotationQuat);
    _rotation.set(rotationQuat);
    dirty();
}

void Transform::setRotation(const Vector3& axis, float angle)
{
    _rotation.set(axis, angle);
    dirty();
}

void Transform::setTranslation(const Vector3& translation)
{
    _translation.set(translation);
    dirty();
}

void Transform::setTranslation(float tx, float ty, float tz)
{
    _translation.set(tx, ty, tz);
    dirty();
}

void Transform::setTranslationX(float tx)
{
    _translation.x = tx;
    dirty();
}

void Transform::setTranslationY(float ty)
{
    _translation.y = ty;
    dirty();
}

void Transform::setTranslationZ(float tz)
{
    _translation.z = tz;
    dirty();
}

void Transform::translate(float tx, float ty, float tz)
{
    _translation.x += tx;
    _translation.y += ty;
    _translation.z += tz;
    dirty();
}

void Transform::translate(const Vector3& translation)
{
    _translation.x += translation.x;
    _translation.y += translation.y;
    _translation.z += translation.z;
    dirty();
}

void Transform::translateX(float tx)
{
    _translation.x += tx;
    dirty();
}

void Transform::translateY(float ty)
{
    _translation.y += ty;
    dirty();
}

void Transform::translateZ(float tz)
{
    _translation.z += tz;
    dirty();
}

void Transform::translateLeft(float amount)
{
    // Force the current transform matrix to be updated.
    getMatrix();

    Vector3 left;
    _matrix.getLeftVector(&left);
    left.normalize();
    left.scale(amount);

    translate(left);
}

void Transform::translateUp(float amount)
{
    // Force the current transform matrix to be updated.
    getMatrix();

    Vector3 up;
    _matrix.getUpVector(&up);
    up.normalize();
    up.scale(amount);

    translate(up);
}

void Transform::translateForward(float amount)
{
    // Force the current transform matrix to be updated.
    getMatrix();

    Vector3 forward;
    _matrix.getForwardVector(&forward);
    forward.normalize();
    forward.scale(amount);

    translate(forward);
}

void Transform::transformPoint(Vector3* point)
{
    assert(point);

    getMatrix();
    _matrix.transformPoint(point);
}

void Transform::transformPoint(const Vector3& point, Vector3* dst)
{
    assert(dst);

    getMatrix();
    _matrix.transformPoint(point, dst);
}

void Transform::transformVector(Vector3* normal)
{
    assert(normal);

    getMatrix();
    _matrix.transformVector(normal);
}

void Transform::transformVector(const Vector3& normal, Vector3* dst)
{
    getMatrix();
    _matrix.transformVector(normal, dst);
}

void Transform::transformVector(float x, float y, float z, float w, Vector3* dst)
{
    getMatrix();
    _matrix.transformVector(x, y, z, w, dst);
}

unsigned int Transform::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
        case ANIMATE_SCALE_UNIT:
        case ANIMATE_SCALE_X:
        case ANIMATE_SCALE_Y:
        case ANIMATE_SCALE_Z:
        case ANIMATE_TRANSLATE_X:
        case ANIMATE_TRANSLATE_Y:
        case ANIMATE_TRANSLATE_Z:
            return 1;
        case ANIMATE_SCALE:
        case ANIMATE_TRANSLATE:
            return 3;
        case ANIMATE_ROTATE:
            return 4;
        case ANIMATE_ROTATE_TRANSLATE:
            return 7;
        case ANIMATE_SCALE_ROTATE_TRANSLATE:
            return 10;
        default:
            return -1;
    }
}

void Transform::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    switch (propertyId)
    {
        case ANIMATE_SCALE_UNIT:
            value->setFloat(0, _scale.x);
            break;
        case ANIMATE_SCALE:
            value->setFloat(0, _scale.x);
            value->setFloat(1, _scale.y);
            value->setFloat(2, _scale.z);
            break;
        case ANIMATE_SCALE_X:
            value->setFloat(0, _scale.x);
            break;
        case ANIMATE_SCALE_Y:
            value->setFloat(0, _scale.y);
            break;
        case ANIMATE_SCALE_Z:
            value->setFloat(0, _scale.z);
            break;
        case ANIMATE_ROTATE:
            value->setFloat(0, _rotation.x);
            value->setFloat(1, _rotation.y);
            value->setFloat(2, _rotation.z);
            value->setFloat(3, _rotation.w);
            break;
        case ANIMATE_TRANSLATE:
            value->setFloat(0, _translation.x);
            value->setFloat(1, _translation.y);
            value->setFloat(2, _translation.z);
            break;
        case ANIMATE_TRANSLATE_X:
            value->setFloat(0, _translation.x);
            break;
        case ANIMATE_TRANSLATE_Y:
            value->setFloat(0, _translation.y);
            break;
        case ANIMATE_TRANSLATE_Z:
            value->setFloat(0, _translation.z);
            break;
        case ANIMATE_ROTATE_TRANSLATE:
            value->setFloat(0, _rotation.x);
            value->setFloat(1, _rotation.y);
            value->setFloat(2, _rotation.z);
            value->setFloat(3, _rotation.w);
            value->setFloat(4, _translation.x);
            value->setFloat(5, _translation.y);
            value->setFloat(6, _translation.z);
            break;
        case ANIMATE_SCALE_ROTATE_TRANSLATE:
            value->setFloat(0, _scale.x);
            value->setFloat(1, _scale.y);
            value->setFloat(2, _scale.z);
            value->setFloat(3, _rotation.x);
            value->setFloat(4, _rotation.y);
            value->setFloat(5, _rotation.z);
            value->setFloat(6, _rotation.w);
            value->setFloat(7, _translation.x);
            value->setFloat(8, _translation.y);
            value->setFloat(9, _translation.z);
            break;
        default:
            break;
    }
}

void Transform::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    assert(blendWeight >= 0.0f && blendWeight <= 1.0f);
    if (blendWeight == 0.0f)
        return;

    switch (propertyId)
    {
        case ANIMATE_SCALE_UNIT:
        {
            float scale = value->getFloat(0);

            if (blendWeight != 1.0f)
                scale *= blendWeight;

            animateScaleX(scale);
            animateScaleY(scale);
            animateScaleZ(scale);
            
            break;
        }   
        case ANIMATE_SCALE:
        {
            float sx = value->getFloat(0);
            float sy = value->getFloat(1);
            float sz = value->getFloat(2);
            if (blendWeight != 1.0f)
            {
                sx *= blendWeight;
                sy *= blendWeight;
                sz *= blendWeight;
            }

            animateScaleX(sx);
            animateScaleY(sy);
            animateScaleZ(sz);

            break;
        }
        case ANIMATE_SCALE_X:
        {
            float sx = value->getFloat(0);

            if (blendWeight != 1.0f)
                sx *= blendWeight;

            animateScaleX(sx);

            break;
        }
        case ANIMATE_SCALE_Y:
        {
            float sy = value->getFloat(0);

            if (blendWeight != 1.0f)
                sy *= blendWeight;

            animateScaleY(sy);

            break;
        }
        case ANIMATE_SCALE_Z:
        {
            float sz = value->getFloat(0);

            if (blendWeight != 1.0f)
                sz *= blendWeight;

            animateScaleZ(sz);

            break;
        }
        case ANIMATE_ROTATE:
        {
            Quaternion q(value->getFloat(0), value->getFloat(1), value->getFloat(2), value->getFloat(3));

            if (blendWeight != 1.0f)
                Quaternion::slerp(Quaternion::identity(), q, blendWeight, &q);

            animateRotate(&q);
            
            break;
        }
        case ANIMATE_TRANSLATE:
        {
            float tx = value->getFloat(0);
            float ty = value->getFloat(1);
            float tz = value->getFloat(2);

            if (blendWeight != 1.0f)
            {
                tx *= blendWeight;
                ty *= blendWeight;
                tz *= blendWeight;
            }

            animateTranslateX(tx);
            animateTranslateY(ty);
            animateTranslateZ(tz);
            
            break;
        }
        case ANIMATE_TRANSLATE_X:
        {
            float tx = value->getFloat(0);

            if (blendWeight != 1.0f)
                tx *= blendWeight;

            animateTranslateX(tx);

            break;
        }
        case ANIMATE_TRANSLATE_Y:
        {
            float ty = value->getFloat(0);

            if (blendWeight != 1.0f)
                ty *= blendWeight;
            
            animateTranslateY(ty);

            break;
        }
        case ANIMATE_TRANSLATE_Z:
        {
            float tz = value->getFloat(0);

            if (blendWeight != 1.0f)
                tz *= blendWeight;

            animateTranslateZ(tz);

            break;
        }
        case ANIMATE_ROTATE_TRANSLATE:
        {
            Quaternion q(value->getFloat(0), value->getFloat(1), value->getFloat(2), value->getFloat(3));
            float tx = value->getFloat(4);
            float ty = value->getFloat(5);
            float tz = value->getFloat(6);

            if (blendWeight != 1.0f)
            {
                Quaternion::slerp(Quaternion::identity(), q, blendWeight, &q);
                tx *= blendWeight;
                ty *= blendWeight;
                tz *= blendWeight;
            }

            animateRotate(&q);
            animateTranslateX(tx);
            animateTranslateY(ty);
            animateTranslateZ(tz);
            
            break;
        }
        case ANIMATE_SCALE_ROTATE_TRANSLATE:
        {
            float sx = value->getFloat(0);
            float sy = value->getFloat(1);
            float sz = value->getFloat(2);
            Quaternion q(value->getFloat(3), value->getFloat(4), value->getFloat(5), value->getFloat(6));
            float tx = value->getFloat(7);
            float ty = value->getFloat(8);
            float tz = value->getFloat(9);

            if (blendWeight != 1.0f)
            {
                sx *= blendWeight;
                sy *= blendWeight;
                sz *= blendWeight;
                Quaternion::slerp(Quaternion::identity(), q, blendWeight, &q);
                tx *= blendWeight;
                ty *= blendWeight;
                tz *= blendWeight;
            }

            animateScaleX(sx);
            animateScaleY(sy);
            animateScaleZ(sz);
            animateRotate(&q);
            animateTranslateX(tx);
            animateTranslateY(ty);
            animateTranslateZ(tz);
            
            break;
        }
        default:
            break;
    }
}

void Transform::dirty()
{
    _matrixDirty = true;
    transformChanged();
}

void Transform::addListener(Transform::Listener* listener, long cookie)
{
    if (_listeners == NULL)
        _listeners = new std::list<TransformListener>();

    TransformListener l;
    l.listener = listener;
    l.cookie = cookie;
    _listeners->push_back(l);
}

void Transform::removeListener(Transform::Listener* listener)
{
    if (_listeners)
    {
        for (std::list<TransformListener>::iterator itr = _listeners->begin(); itr != _listeners->end(); itr++)
        {
            if ((*itr).listener == listener)
            {
                _listeners->erase(itr);
                break;
            }
        }
    }
}

void Transform::transformChanged()
{
    if (_listeners)
    {
        for (std::list<TransformListener>::iterator itr = _listeners->begin(); itr != _listeners->end(); itr++)
        {
            TransformListener& l = *itr;
            l.listener->transformChanged(this, l.cookie);
        }
    }
}


void Transform::animateScaleX(float sx)
{
    if ((_bitFlag & SCALE_X_BIT) != SCALE_X_BIT)
    {
        _bitFlag |= SCALE_X_BIT;
        setScaleX(sx);
    }
    else
    {
        _scale.x += sx;
        dirty();
    }
}

void Transform::animateScaleY(float sy)
{
    if ((_bitFlag & SCALE_Y_BIT) != SCALE_Y_BIT)
    {
        _bitFlag |= SCALE_Y_BIT;
        setScaleY(sy);
    }
    else
    {
        _scale.y += sy;
        dirty();
    }
}

void Transform::animateScaleZ(float sz)
{
    if ((_bitFlag & SCALE_Z_BIT) != SCALE_Z_BIT)
    {
        _bitFlag |= SCALE_Z_BIT;
        setScaleZ(sz);
    }
    else
    {
        _scale.z += sz;
        dirty();
    }
}

void Transform::animateRotate(Quaternion* q)
{
    if ((_bitFlag & ROTATE_BIT) != ROTATE_BIT)
    {
        _bitFlag |= ROTATE_BIT;
        setRotation(*q);
    }
    else
    {
        rotate(*q);
    }
}

void Transform::animateTranslateX(float tx)
{
    if ((_bitFlag & TRANSLATE_X_BIT) != TRANSLATE_X_BIT)
    {
        _bitFlag |= TRANSLATE_X_BIT;
        setTranslationX(tx);
    }
    else
    {
        translateX(tx);
    }
}

void Transform::animateTranslateY(float ty)
{
    if ((_bitFlag & TRANSLATE_Y_BIT) != TRANSLATE_Y_BIT)
    {
        _bitFlag |= TRANSLATE_Y_BIT;
        setTranslationY(ty);
    }
    else
    {
        translateY(ty);
    }
}

void Transform::animateTranslateZ(float tz)
{
    if ((_bitFlag & TRANSLATE_Z_BIT) != TRANSLATE_Z_BIT)
    {
        _bitFlag |= TRANSLATE_Z_BIT;
        setTranslationZ(tz);
    }
    else
    {
        translateZ(tz);
    }
}



}
