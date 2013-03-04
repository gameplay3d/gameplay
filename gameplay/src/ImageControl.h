#ifndef IMAGECONTROL_H_
#define IMAGECONTROL_H_

#include "Control.h"
#include "Theme.h"
#include "Image.h"
#include "SpriteBatch.h"
#include "Rectangle.h"

namespace gameplay
{

/**
 * An ImageControl allows forms to display images from arbitrary files not specified in the theme.
 *
 * The following properties are available for image controls:

 @verbatim
     imageControl <control ID>
     {
         style          = <styleID>
         alignment      = <Control::Alignment constant> // Note: 'position' will be ignored.
         position       = <x, y>
         autoWidth      = <bool>
         autoHeight     = <bool>
         size           = <width, height>
         width          = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
         height         = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
         consumeEvents  = <bool>    // Whether the label propagates input events to the Game's input event handler. Default is true.
         path           = <string>  // Path to image or texture atlas.
         srcRegion      = <x, y, width, height>  // Region within file to create UVs from.
         dstRegion      = <x, y, width, height>  // Region of control's viewport to draw into.
     }
 @endverbatim
 */
class ImageControl : public Control
{
    friend class Container;

public:

    static ImageControl* create(const char* id, Theme::Style* style);

    void setImage(const char* path);
    
    void setImage(Image* image);

    void setSrcRegion(float x, float y, float width, float height);

    const Rectangle& getSrcRegion() const;

    void setDstRegion(float x, float y, float width, float height);

    const Rectangle& getDstRegion() const;

    Image* getImage() const;

protected:

    ImageControl();
    
    virtual ~ImageControl();

    static ImageControl* create(Theme::Style* style, Properties* properties);

    virtual void initialize(Theme::Style* style, Properties* properties);

    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /* Source region: Region of the file to use as a single image. */
    Rectangle _srcRegion;
    /* Destination region: Region within the control to draw the image. */
    Rectangle _dstRegion;
    /* The SpriteBatch used to draw the image. */
    SpriteBatch* _batch;
    
    /* One over texture width and height, for use when calculating UVs from a new source region. */
    float _tw;
    float _th;
    
    /* Calculated UVs. */
    Theme::UVs _uvs;

private:

    ImageControl(const ImageControl& copy);
};

}

#endif