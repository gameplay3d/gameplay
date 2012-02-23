#include "Base.h"
#include "Control.h"

namespace gameplay
{
    Control::Control()
        : _id(""), _state(Control::STATE_NORMAL), _size(Vector2::zero()), _position(Vector2::zero()), _border(Vector2::zero()), _padding(Vector2::zero()),
          _autoWidth(true), _autoHeight(true), _dirty(true)
    {
    }

    Control::Control(const Control& copy)
    {
    }

    Control::~Control()
    {
    }

    const char* Control::getID()
    {
        return _id.c_str();
    }

    const Rectangle Control::getBounds(bool includePadding) const
    {
        // TODO
        return Rectangle();
    }

    void Control::setPosition(float x, float y)
    {
        _position.set(x, y);
    }

    const Vector2& Control::getPosition() const
    {
        return _position;
    }

    void Control::setSize(float width, float height)
    {
        _size.set(width, height);
    }

    const Vector2& Control::getSize() const
    {
        return _size;
    }

    void Control::setAutoSize(bool width, bool height)
    {
        _autoWidth = width;
        _autoHeight = height;
    }

    void Control::setStyle(Theme::Style* style)
    {
        _style = style;
    }

    Theme::Style* Control::getStyle() const
    {
        return _style;
    }

    void Control::setState(State state)
    {
        _state = state;
    }

    Control::State Control::getState()
    {
        return _state;
    }

    void Control::disable()
    {
        _state = STATE_DISABLED;
    }

    void Control::enable()
    {
        _state = STATE_NORMAL;
    }

    Theme::Style::OverlayType Control::getOverlayType() const
    {
        switch (_state)
        {
        case Control::STATE_NORMAL:
            return Theme::Style::OVERLAY_NORMAL;
        case Control::STATE_FOCUS:
            return Theme::Style::OVERLAY_FOCUS;
        case Control::STATE_ACTIVE:
            return Theme::Style::OVERLAY_ACTIVE;
        default:
            return Theme::Style::OVERLAY_NORMAL;
        }
    }

    void Control::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        // Empty stub to be implemented by Button and its descendents.
    }

    void Control::keyEvent(Keyboard::KeyEvent evt, int key)
    {
    }

    void Control::update(const Vector2& position)
    {
    }

    void Control::drawBorder(SpriteBatch* spriteBatch, const Vector2& position)
    {
        Vector2 pos(position.x + _position.x, position.y + _position.y);

        // Get the border and background images for this control's current state.
        Theme::ContainerRegion* containerRegion = _style->getOverlay(getOverlayType())->getContainerRegion();
        if (containerRegion)
        {
            // Get the UVs.
            Theme::UVs topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight;
            topLeft = containerRegion->getUVs(Theme::ContainerRegion::TOP_LEFT);
            top = containerRegion->getUVs(Theme::ContainerRegion::TOP);
            topRight = containerRegion->getUVs(Theme::ContainerRegion::TOP_RIGHT);
            left = containerRegion->getUVs(Theme::ContainerRegion::LEFT);
            center = containerRegion->getUVs(Theme::ContainerRegion::CENTER);
            right = containerRegion->getUVs(Theme::ContainerRegion::RIGHT);
            bottomLeft = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM_LEFT);
            bottom = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM);
            bottomRight = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM_RIGHT);

            // Calculate screen-space positions.
            Theme::Border border = containerRegion->getBorder();
            Theme::Padding padding = _style->getPadding();
            Vector4 borderColor = containerRegion->getColor();

            float midWidth = _size.x - border.left - border.right;
            float midHeight = _size.y - border.top - border.bottom;
            float midX = pos.x + border.left;
            float midY = pos.y + border.top;
            float rightX = pos.x + _size.x - border.right;
            float bottomY = pos.y + _size.y - border.bottom;

            // Draw themed border sprites.
            if (border.left && border.top)
                spriteBatch->draw(pos.x, pos.y, border.left, border.top, topLeft.u1, topLeft.v1, topLeft.u2, topLeft.v2, borderColor);
            if (border.top)
                spriteBatch->draw(pos.x + border.left, pos.y, midWidth, border.top, top.u1, top.v1, top.u2, top.v2, borderColor);
            if (border.right && border.top)
                spriteBatch->draw(rightX, pos.y, border.right, border.top, topRight.u1, topRight.v1, topRight.u2, topRight.v2, borderColor);
            if (border.left)
                spriteBatch->draw(pos.x, midY, border.left, midHeight, left.u1, left.v1, left.u2, left.v2, borderColor);
            
            spriteBatch->draw(pos.x + border.left, pos.y + border.top, _size.x - border.left - border.right, _size.y - border.top - border.bottom,
                center.u1, center.v1, center.u2, center.v2, borderColor);

            if (border.right)
                spriteBatch->draw(rightX, midY, border.right, midHeight, right.u1, right.v1, right.u2, right.v2, borderColor);
            if (border.bottom && border.left)
                spriteBatch->draw(pos.x, bottomY, border.left, border.bottom, bottomLeft.u1, bottomLeft.v1, bottomLeft.u2, bottomLeft.v2, borderColor);
            if (border.bottom)
                spriteBatch->draw(midX, bottomY, midWidth, border.bottom, bottom.u1, bottom.v1, bottom.u2, bottom.v2, borderColor);
            if (border.bottom && border.right)
                spriteBatch->draw(rightX, bottomY, border.right, border.bottom, bottomRight.u1, bottomRight.v1, bottomRight.u2, bottomRight.v2, borderColor);
        }
    }

    void Control::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
    {
    }

    void Control::drawText(const Vector2& position)
    {
    }

    bool Control::isDirty()
    {
        return _dirty;
    }
}
