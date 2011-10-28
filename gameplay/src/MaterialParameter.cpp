/*
 * MaterialParameter.cpp
 */

#include "Base.h"
#include "MaterialParameter.h"

namespace gameplay
{

MaterialParameter::MaterialParameter(Material* material, Uniform* uniform) :
    _type(MaterialParameter::NONE), _count(1), _dynamic(false), _uniform(uniform), _material(material)
{
    clearValue();
}

MaterialParameter::~MaterialParameter()
{
    clearValue();
}

void MaterialParameter::clearValue()
{
    if (_dynamic)
    {
        switch (_type)
        {
        case MaterialParameter::FLOAT:
        case MaterialParameter::VECTOR2:
        case MaterialParameter::VECTOR3:
        case MaterialParameter::VECTOR4:
        case MaterialParameter::MATRIX:
            if (_value.floatPtrValue)
            {
                delete[] _value.floatPtrValue;
            }
            break;
        case MaterialParameter::INT:
            if (_value.intPtrValue)
            {
                delete[] _value.intPtrValue;
            }
            break;
        case MaterialParameter::METHOD:
            if (_value.method)
            {
                delete _value.method;
            }
        }

        _dynamic = false;
        _count = 1;
    }
    else
    {
        switch (_type)
        {
        case MaterialParameter::TEXTURE:
            if (_value.textureValue)
            {
                const_cast<Texture*> (_value.textureValue)->release();
            }
            break;
        }
    }

    memset(&_value, 0, sizeof(_value));
    _type = MaterialParameter::NONE;
}

void MaterialParameter::setValue(float value)
{
    clearValue();

    _value.floatValue = value;
    _type = MaterialParameter::FLOAT;
}

void MaterialParameter::setValue(int value)
{
    clearValue();

    _value.intValue = value;
    _type = MaterialParameter::INT;
}

void MaterialParameter::setValue(const float* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (values);
    _count = count;
    _type = MaterialParameter::FLOAT;
}

void MaterialParameter::setValue(const int* values, unsigned int count)
{
    clearValue();

    _value.intPtrValue = const_cast<int*> (values);
    _count = count;
    _type = MaterialParameter::INT;
}

void MaterialParameter::setValue(const Color& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[4];
    memcpy(array, &value.r, sizeof(float) * 4);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const Color* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].r);
    _count = count * 4; // 4 elements per color
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const Vector2& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[2];
    memcpy(array, &value.x, sizeof(float) * 2);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::setValue(const Vector2* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR2;
}

void MaterialParameter::setValue(const Vector3& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[3];
    memcpy(array, &value.x, sizeof(float) * 3);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::setValue(const Vector3* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR3;
}

void MaterialParameter::setValue(const Vector4& value)
{
    clearValue();

    // Copy data by-value into a dynamic array.
    float* array = new float[4];
    memcpy(array, &value.x, sizeof(float) * 4);

    _value.floatPtrValue = array;
    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const Vector4* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<float*> (&values[0].x);
    _count = count;
    _type = MaterialParameter::VECTOR4;
}

void MaterialParameter::setValue(const Matrix& value)
{
    // If this parameter is already storing a single dynamic matrix, no need to clear it.
    if (!(_dynamic && _count == 1 && _type == MaterialParameter::MATRIX && _value.floatPtrValue != NULL))
    {
        clearValue();

        // Allocate a new dynamic matrix.
        _value.floatPtrValue = new float[16];
    }

    memcpy(_value.floatPtrValue, value.m, sizeof(float) * 16);

    _dynamic = true;
    _count = 1;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::setValue(const Matrix* values, unsigned int count)
{
    clearValue();

    _value.floatPtrValue = const_cast<Matrix&> (values[0]).m;
    _count = count;
    _type = MaterialParameter::MATRIX;
}

void MaterialParameter::setValue(const Texture* texture)
{
    clearValue();

    if (texture)
    {
        const_cast<Texture*>(texture)->addRef();
        _value.textureValue = texture;
        _type = MaterialParameter::TEXTURE;
    }
}

void MaterialParameter::setValue(const char* texturePath, bool generateMipmaps)
{
    if (texturePath)
    {
        clearValue();

        Texture* texture = Texture::create(texturePath, generateMipmaps);
        if (texture)
        {
            _value.textureValue = texture;
            _type = MaterialParameter::TEXTURE;
        }
    }
}

void MaterialParameter::bind()
{
    Effect* effect = _material->getEffect();

    switch (_type)
    {
    case MaterialParameter::FLOAT:
        if (_count == 1)
        {
            effect->setValue(_uniform, _value.floatValue);
        }
        else
        {
            assert(_value.floatPtrValue);
            effect->setValue(_uniform, _value.floatPtrValue, _count);
        }
        break;
    case MaterialParameter::INT:
        if (_count == 1)
        {
            effect->setValue(_uniform, _value.intValue);
        }
        else
        {
            assert(_value.intPtrValue);
            effect->setValue(_uniform, _value.intPtrValue, _count);
        }
        break;
    case MaterialParameter::VECTOR2:
        assert(_value.floatPtrValue);
        effect->setValue(_uniform, reinterpret_cast<Vector2*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR3:
        assert(_value.floatPtrValue);
        effect->setValue(_uniform, reinterpret_cast<Vector3*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::VECTOR4:
        assert(_value.floatPtrValue);
        effect->setValue(_uniform, reinterpret_cast<Vector4*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::MATRIX:
        assert(_value.floatPtrValue);
        effect->setValue(_uniform, reinterpret_cast<Matrix*>(_value.floatPtrValue), _count);
        break;
    case MaterialParameter::TEXTURE:
        assert(_value.textureValue);
        effect->setValue(_uniform, _value.textureValue);
        break;
    case MaterialParameter::METHOD:
        assert(_value.method);
        _value.method->setValue();
        break;
    }
}

unsigned int MaterialParameter::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
    case ANIMATE_UNIFORM:
        switch(_type)
        {
            // These types don't support animation.
            case NONE:
            case MATRIX:
            case TEXTURE:
            case METHOD:
                return 0;
            default:
                return _count;
        }
    }

    return 0;
}

void MaterialParameter::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    switch (propertyId)
    {
    case ANIMATE_UNIFORM:
        switch (_type)
        {
        case FLOAT:
            value->setFloat(0, _value.floatValue);
            break;
        case INT:
            value->setFloat(0, _value.intValue);
            break;
        case VECTOR2:
        case VECTOR3:
        case VECTOR4:
            value->setFloat(_value.floatPtrValue, 0, _count);
            break;
        default:
            // UNSUPPORTED: NONE, MATRIX, METHOD, TEXTURE 
            break;
        }
        break;
    }
}

void MaterialParameter::setAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    switch (propertyId)
    {
    case ANIMATE_UNIFORM:
        switch (_type)
        {
        case FLOAT:
            _value.floatValue = value->getFloat(0);
            break;
        case INT:
            _value.intValue = value->getFloat(0);
            break;
        case VECTOR2:
        case VECTOR3:
        case VECTOR4:
            value->getFloat(_value.floatPtrValue, 0, _count);
            break;
        default:
            // UNSUPPORTED: NONE, MATRIX, METHOD, TEXTURE 
            break;
        }
        break;
    }
}

}
