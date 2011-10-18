/*
 * Viewport.h
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

namespace gameplay
{

class Camera;

/**
 * Defines a rectangular viewing region used by camera the project into
 * and used by the GraphicsDevice to control the rendering region.
 */
class Viewport
{
public:

    /**
     * Constructs a new viewport with all zeros.
     */
    Viewport();

    /**
     * Constructs a new viewport with the specified dimensions.
     *
     * @param x The x-coordinate of the viewport.
     * @param y The y-coordinate of the viewport.
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     */
    Viewport(int x, int y, int width, int height);

    /**
     * Constructs a new viewport from a copy.
     *
     * @param copy The viewport to copy.
     */
    Viewport(const Viewport& copy);

    /**
     * Destructor.
     */
    ~Viewport();

    /**
     * Sets the viewport to the specified dimensions.
     *
     * @param x The x-coordinate of the viewport.
     * @param y The y-coordinate of the viewport.
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     */
    void set(int x, int y, int width, int height);

    /**
     * Sets the viewport to the specified viewport copy.
     *
     * @param viewport The viewport to copy.
     */
    void set(const Viewport& viewport);

    /**
     * Gets the x-coordinate of the viewport.
     *
     * @return The x-coordinate of the viewport.
     */
    int getX() const;

    /**
     * Sets the x-coordinate of the viewport.
     *
     * @param x The x-coordinate of the viewport.
     */
    void setX(int x);

    /**
     * Gets the y-coordinate of the viewport.
     *
     * @return The y-coordinate of the viewport.
     */
    int getY() const;

    /**
     * Sets the y-coordinate of the viewport.
     *
     * @param y The y-coordinate of the viewport.
     */
    void setY(int y);

    /**
     * Gets the width of the viewport.
     *
     * @return The width of the viewport.
     */
    int getWidth() const;

    /**
     * Sets the width of the viewport.
     *
     * @param width The width of the viewport.
     */
    void setWidth(int width);

    /**
     * Gets the height of the viewport.
     *
     * @return The height of the viewport.
     */
    int getHeight() const;

    /**
     * Sets the height of the viewport.
     *
     * @param height The height of the viewport.
     */
    void setHeight(int height);

    /**
     * Makes this the active viewport for rendering.
     */
    void bind();

private:

    int _x;
    int _y;
    int _width;
    int _height;
};

}

#endif
