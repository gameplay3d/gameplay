#ifndef IMAGECONTROL_H_
#define IMAGECONTROL_H_

#include "Button.h"
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
     image <control ID>
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
class ImageControl : public Button
{
    friend class Container;

public:

    /**
     * Create a new ImageControl.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new ImageControl.
     * @script{create}
     */
    static ImageControl* create(const char* id, Theme::Style* style);

    /**
     * Set the path of the image for this ImageControl to display.
     *
     * @param path The path to the image.
     */
    void setImage(const char* path);

    /**
     * Set the source region of this ImageControl.  This is the region of the file,
     * in pixels, to use when drawing.
     *
     * @param x The x coordinate of the source region.
     * @param y The y coordinate of the source region.
     * @param width The width of the source region.
     * @param height The height of the source region.
     */
    void setRegionSrc(float x, float y, float width, float height);

    /**
     * Set the source region of this ImageControl.  This is the region of the file,
     * in pixels, to use when drawing.
     *
     * @param region The new source region.
     */
    void setRegionSrc(const Rectangle& region);

    /**
     * Get the source region of this ImageControl.
     *
     * @return The source region of this ImageControl.
     */
    const Rectangle& getRegionSrc() const;

    /**
     * Sets the destination region of this ImageControl.  This is the region
     * within the control's viewport to draw the image.
     *
     * @param x The x coordinate of the destination region.
     * @param y The y coordinate of the destination region.
     * @param width The width of the destination region.
     * @param height The height of the destination region.
     */
    void setRegionDst(float x, float y, float width, float height);
    
    /**
     * Sets the destination region of this ImageControl.  This is the region
     * within the control's viewport to draw the image.
     *
     * @param region The new destination region.
     */
    void setRegionDst(const Rectangle& region);

    /**
     * Get the destination region of this ImageControl.
     *
     * @return The destination region of this ImageControl.
     */
    const Rectangle& getRegionDst() const;

    const char* getType() const;

protected:

    ImageControl();
    
    virtual ~ImageControl();

    static ImageControl* create(Theme::Style* style, Properties* properties);

    virtual void initialize(Theme::Style* style, Properties* properties);

    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    // Source region.
    Rectangle _srcRegion;
    // Destination region.
    Rectangle _dstRegion;
    SpriteBatch* _batch;
    
    // One over texture width and height, for use when calculating UVs from a new source region.
    float _tw;
    float _th;
    
    // Calculated UVs.
    Theme::UVs _uvs;

private:

    ImageControl(const ImageControl& copy);
};

}

#endif