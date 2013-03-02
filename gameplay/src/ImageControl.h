#ifndef IMAGECONTROL_H_
#define IMAGECONTROL_H_

#include "Control.h"
#include "Theme.h"
#include "Image.h"
#include "SpriteBatch.h"

namespace gameplay
{

class ImageControl : public Control
{
    friend class Container;

public:

    static ImageControl* create(const char* id, Theme::Style* style);

    void setImage(const char* path);
    void setImage(Image* image);
    //void setImageData(const char* data);

    Image* getImage() const;

protected:

    ImageControl();
    
    virtual ~ImageControl();

    static ImageControl* create(Theme::Style* style, Properties* properties);

    virtual void initialize(Theme::Style* style, Properties* properties);

    //void update(const Control* container, const Vector2& offset);

    //void draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight);
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    Image* _image;
    Texture* _texture;
    SpriteBatch* _batch;

private:

    ImageControl(const ImageControl& copy);
};

}

#endif