#include "Image.h"

namespace gameplay
{

inline unsigned char* Image::getData() const
{
    return _data;
}

inline Image::Format Image::getFormat() const
{
    return _format;
}

inline unsigned int Image::getHeight() const
{
    return _height;
}
        
inline unsigned int Image::getWidth() const
{
    return _width;
}

}
