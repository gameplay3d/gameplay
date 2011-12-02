/*
 * Theme.h
 */

# ifndef THEME_H_
# define THEME_H_

#include "Base.h"
#include "Ref.h"
#include "Font.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Properties.h"

namespace gameplay
{

#define MAX_OVERLAYS 3
/**
 * This class represents the apperance of an UI form described in a 
 * theme file. A theme file, at its simplest, contains a source texture atlas,
 * the texture coordinates of each control in its different mode. Once loaded,
 * the appearance properties can be retrieved using a style id and set on a UI control.
 */
class Theme: public Ref
{
public:
    class Style;
    class Cursor;

    typedef struct UVs
    {
        float u1;
        float v1;
        float u2;
        float v2;
    } UVs;

    typedef struct padding
    {
        float top;
        float bottom;
        float left;
        float right;
    } Border, Padding;

    /**
     * Creates an instance of a Theme from a theme file.
     *
     * @param path path to a theme file.
     *
     * @return A new Theme.
     */
    static Theme* create(const char* path);

    /**
     * Returns style with the given name.
     *
     * @param name Name of the style (as specified in the Theme file).
     *
     * @return instance of the Style.
     */
    const Theme::Style* getStyle(const char* styleName) const;

    /**
     * This class represents the apperance of a cursor.
     */
    class Cursor
    {
    public:
       /**
        * Returns the Id of this Cursor.
        */
        const char* getId() const;

       /**
        * Gets a texture region in the texture atlas for a cursor.
        */
        const Rectangle& getRegion() const;

       /**
        * Gets a UV coordinates computed from the texture region.
        */
        const Theme::UVs& getUVs() const;
    
    private:

        std::string _id;
        Rectangle _region;
        UVs _uvs;
    };
    
    /**
     * This class represents the apperance of a control's style.
     */
    class Style
    {
    public:
        class Overlay;

        enum OverlayType
        {
            OVERLAY_NORMAL,
            OVERLAY_FOCUS,
            OVERLAY_ACTIVE
        };

        /**
         * Returns the Id of this Style.
         */
        const char* getId() const;

        /**
         * Gets an overlay from the overlay type.
         */
        const Theme::Style::Overlay& getOverlay(OverlayType overlayType) const;

       /**
        * Gets a Border region of this overlay.
        */
        const Theme::Border& getBorder() const;

        /**
        * Gets a Padding region of this overlay.
        */
        const Theme::Padding& getPadding() const;
       
        /**
         * This class represents a control's overlay for one of the 3 modes: normal, focussed or active.
         */
        class Overlay
        {
        public:
           /**
            * Destructor.
            */
            ~Overlay();

           /**
            * Returns the Overlay type.
            */
            OverlayType getId();
            
            /**
            * Gets a texture region in the texture atlas that the overlay uses.
            */
            const Rectangle& getRegion() const;

           /**
            * Gets a UV coordinates computed from the texture region.
            */
            const Theme::UVs& getUVs() const;

           /**
            * Gets a font associated with this overlay.
            */
            const Font* getFont() const;

           /**
            * Gets a cursor associated with this overlay.
            */
            const Cursor& getCursor() const;
        
        private:
           
            OverlayType _id;
            Rectangle _region;
            UVs _uvs;
            Font* _font;
            Cursor _cursor;
        };

    private:
        
        std::string _id;
        Border _border;
        Padding _padding;
        Overlay _overlays[MAX_OVERLAYS];
    };

private:
    /**
     * Constructor.
     */
    Theme();

    /**
     * Copy Constructor.
     */
    Theme(const Theme* theme);

    /**
     * Destructor.
     */
    ~Theme();

    Texture* _texture;
    std::vector<Cursor *> _cursors;
    std::vector<Style *> _styles;
    std::vector<Font *> _fonts;
};

}

#endif