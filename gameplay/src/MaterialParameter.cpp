#include "Base.h"
#include "MaterialParameter.h"

namespace gameplay
{

MaterialParameter::MaterialParameter(const char* name) :
    _type(MaterialParameter::NONE), _count(1), _dynamic(false), _name(name), _uniform(NULL)
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
            SAFE_DELETE_ARRAY(_value.floatPtrValue);
            break;
        case MaterialParameter::INT:
            SAFE_DELETE_ARRAY(_value.intPtrValue);
            break;
        case MaterialParameter::METHOD:
            SAFE_RELEASE(_value.method);
            break;
        }

        _dynamic = false;
        _count = 1;
    }
    else
    {
        switch (_type)
        {
        case MaterialParameter::SAMPLER:
            if (_value.samplerValue)
            {
                const_cast<Texture::Sampler*>(_value.samplerValue)->release();
            }
            break;
        }
    }

    memset(&_value, 0, sizeof(_value));
    _type = MaterialParameter::NONE;
}

const char* MaterialParameter::getName() const
{
    return _name.c_str();
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

void MaterialParameter::setValue(const Texture::Sampler* sampler)
{
    clearValue();

    if (sampler)
    {
        const_cast<Texture::Sampler*>(sampler)->addRef();
        _value.samplerValue = sampler;
        _type = MaterialParameter::SAMPLER;
    }
}

Texture::Sampler* MaterialParameter::setValue(const char* texturePath, bool generateMipmaps)
{
    if (texturePath)
    {
        clearValue();

        Texture::Sampler* sampler = Texture::Sampler::create(texturePath, generateMipmaps);
        if (sampler)
        {
            _value.samplerValue = sampler;
            _type = MaterialParameter::SAMPLER;
        }
        return sampler;
    }

    return NULL;
}

void MaterialParameter::bind(Effect* effect)
{
    // If we had a Uniform cached that is not from the passed in effect,
    // we need to update our uniform to point to the new effect's uniform.
    if (!_uniform || _uniform->getEffect() != effect)
    {
        _uniform = effect->getUniform(_name.c_str());

        if (!_uniform)
        {
            // This parameter was not found in the specified effect, so do nothing.
            WARN_VARG("Warning: Material parameter '%s' not found in effect '%s'.", _name.c_str(), effect->getId());
            return;
        }
    }

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
    case MaterialParameter::SAMPLER:
        assert(_value.samplerValue);
        effect->setValue(_uniform, _value.samplerValue);
        break;
    case MaterialParameter::METHOD:
        assert(_value.method);
        _value.method->setValue(effect);
        break;
    }
}

unsigned int MaterialParameter::getAnimationPropertyComponentCount(int propertyId) const
{
    switch (propertyId)
    {
        case ANIMATE_UNIFORM:
        {
            switch (_type)
            {
                // These types don't support animation.
                case NONE:
                case MATRIX:
                case SAMPLER:
                case METHOD:
                    return 0;
                case FLOAT:
                case INT:
                    return _count;
                case VECTOR2:
                    return 2 * _count;
                case VECTOR3:
                    return 3 * _count;
                case VECTOR4:
                    return 4 * _count;
                default:
                    return 0;
            }
        }
        break;
    }

    return 0;
}

void MaterialParameter::getAnimationPropertyValue(int propertyId, AnimationValue* value)
{
    switch (propertyId)
    {
        case ANIMATE_UNIFORM:
        {
            switch (_type)
            {
                case FLOAT:
                    if (_count == 1)
                    {
                        value->setFloat(0, _value.floatValue);
                    }
                    else
                    {
                        for (unsigned int i = 0; i < _count; i++)
                        {
                            value->setFloat(i, _value.floatPtrValue[i]);
                        }
                    }
                    break;
                case INT:
                    if (_count == 1)
                    {
                        value->setFloat(0, _value.intValue);
                    }
                    else
                    {
                        for (unsigned int i = 0; i < _count; i++)
                        {
                            value->setFloat(i, _value.intPtrValue[i]);
                        }
                    }
                    break;
                case VECTOR2:
                    for (unsigned int i = 0; i < _count; i++)
                    {
                        value->setFloat(_value.floatPtrValue, i * 2, 2);
                    }
                    break;
                case VECTOR3:
                    for (unsigned int i = 0; i < _count; i++)
                    {
                        value->setFloat(_value.floatPtrValue, i * 3, 3);
                    }
                    break;
                case VECTOR4:
                    for (unsigned int i = 0; i < _count; i++)
                    {
                        value->setFloat(_value.floatPtrValue, i * 4, 4);
                    }
                    break;

                // UNSUPPORTED: NONE, MATRIX, METHOD, SAMPLER 
            }
        }
        break;
    }
}

void MaterialParameter::setAnimationPropertyValue(int propertyId, AnimationValue* value, float blendWeight)
{
    assert(blendWeight >= 0.0f && blendWeight <= 1.0f);

    switch (propertyId)
    {
        case ANIMATE_UNIFORM:
        {
            switch (_type)
            {
                case FLOAT:
                {
                    if (_count == 1)
                        _value.floatValue = Curve::lerp(blendWeight, _value.floatValue, value->getFloat(0));
                    else
                        applyAnimationValue(value, blendWeight, 1);
                    break;
                }
                case INT:
                {
                    if (_count == 1)
                    {
                        _value.intValue = Curve::lerp(blendWeight, _value.intValue, value->getFloat(0));
                    }
                    else
                    {
                        for (unsigned int i = 0; i < _count; i++)
                            _value.intPtrValue[i] = Curve::lerp(blendWeight, _value.intPtrValue[i], value->getFloat(i));
                    }
                    break;
                }
                case VECTOR2:
                {
                    applyAnimationValue(value, blendWeight, 2);
                    break;
                }
                case VECTOR3:
                {
                    applyAnimationValue(value, blendWeight, 3);
                    break;
                }
                case VECTOR4:
                {
                    applyAnimationValue(value, blendWeight, 4);
                    break;
                }
                // UNSUPPORTED: NONE, MATRIX, METHOD, SAMPLER 
            }
        }
        break;
    }
}

void MaterialParameter::applyAnimationValue(AnimationValue* value, float blendWeight, int components)
{
    unsigned int count = _count * components;
    for (unsigned int i = 0; i < count; i++)
        _value.floatPtrValue[i] = Curve::lerp(blendWeight, _value.floatPtrValue[i], value->getFloat(i));
}

void MaterialParameter::cloneInto(MaterialParameter* materialParameter) const
{
    materialParameter->_type = _type;
    materialParameter->_count = _count;
    materialParameter->_dynamic = _dynamic;
    materialParameter->_uniform = _uniform;
    switch (_type)
    {
    case NONE:
        break;
    case FLOAT:
        materialParameter->setValue(_value.floatValue);
        break;
    case INT:
        materialParameter->setValue(_value.intValue);
        break;
    case VECTOR2:
    {
        Vector2* value = reinterpret_cast<Vector2*>(_value.floatPtrValue);
        if (_count == 1)
        {
            materialParameter->setValue(*value);
        }
        else
        {
            materialParameter->setValue(value, _count);
        }
        break;
    }   
    case VECTOR3:
    {
        Vector3* value = reinterpret_cast<Vector3*>(_value.floatPtrValue);
        if (_count == 1)
        {
            materialParameter->setValue(*value);
        }
        else
        {
            materialParameter->setValue(value, _count);
        }
        break;
    }
    case VECTOR4:
    {
        Vector4* value = reinterpret_cast<Vector4*>(_value.floatPtrValue);
        if (_count == 1)
        {
            materialParameter->setValue(*value);
        }
        else
        {
            materialParameter->setValue(value, _count);
        }
        break;
    }
    case MATRIX:
    {
        Matrix* value = reinterpret_cast<Matrix*>(_value.floatPtrValue);
        if (_count == 1)
        {
            materialParameter->setValue(*value);
        }
        else
        {
            materialParameter->setValue(value, _count);
        }
        break;
    }
    case SAMPLER:
        materialParameter->setValue(_value.samplerValue);
        break;
    case METHOD:
        materialParameter->_value.method = _value.method;
        materialParameter->_value.method->addRef();
        break;
    }
}

}
