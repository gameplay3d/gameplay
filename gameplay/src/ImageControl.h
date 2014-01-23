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
 * Defines an image control.
 *
 * This allows forms to display seperate images from arbitrary files not specified in the theme.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class ImageControl : public Control
{
    friend class Container;
    friend class ControlFactory;

public:

    /**
     * Creates a new ImageControl.
     *
     * @param id The image control ID.
     * @param style The image control style (optional).
     *
     * @return The new image control.
     * @script{create}
     */
    static ImageControl* create(const char* id, Theme::Style* style = NULL);

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

    /**
     * Creates a new ImageControl.
     * 
     * @param style The control's custom style.
     * @param properties A properties object containing a definition of the ImageControl (optional).
     *
     * @return The new ImageControl.
     * @script{create}
      * 
      */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * @see Control::updateBounds
     */
    void updateBounds();

private:

    ImageControl(const ImageControl& copy);

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
};

}

#endif