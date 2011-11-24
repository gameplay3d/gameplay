#ifndef ANIMATIONCHANNEL_H_
#define ANIMATIONCHANNEL_H_

#include "Object.h"

namespace gameplay
{

class AnimationChannel : public Object
{
public:

    enum InterpolationTypes
    {
        LINEAR = 1,
        BEZIER = 2,
        CARDINAL = 3,
        HERMITE = 4,
        BSPLINE = 5,
        STEP = 6
    };

    /**
     * Constructor.
     */
    AnimationChannel(void);

    /**
     * Destructor.
     */
    virtual ~AnimationChannel(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Sets the interpolation type of the entire animation channel.
     * 
     * @param interpolation The interpolation type from InterpolationTypes enum.
     */
    void setInterpolation(unsigned int interpolation);

    void setTargetId(const std::string str);
    void setTargetAttribute(unsigned int attrib);

    void setKeyTimes(const std::vector<float>& values);
    void setKeyValues(const std::vector<float>& values);
    void setTangentsIn(const std::vector<float>& values);
    void setTangentsOut(const std::vector<float>& values);
    void setInterpolations(const std::vector<unsigned int>& values);

    const std::vector<float>& getKeyValues() const;

    void removeDuplicates();

    void convertToQuaternion();
    void convertToTransform();

    /**
     * Returns the interpolation type value for the given string or zero if not valid.
     * Example: "LINEAR" returns AnimationChannel::LINEAR
     * 
     * @param str Interpolation such as "LINEAR" or "BSPLINE".
     * 
     * @return A value from InterpolationTypes enum or zero if not valid.
     */
    static unsigned int getInterpolationType(const char* str);

private:

    void deleteRange(size_t begin, size_t end);

private:

    std::string _targetId;
    unsigned int _targetAttrib;
    std::vector<float> _keytimes;
    std::vector<float> _keyValues;
    std::vector<float> _tangentsIn;
    std::vector<float> _tangentsOut;
    std::vector<unsigned int> _interpolations;
};


}
#endif

