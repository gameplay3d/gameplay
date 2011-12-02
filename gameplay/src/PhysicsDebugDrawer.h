#ifndef PHYSICSDEBUGDRAWER_H_
#define PHYSICSDEBUGDRAWER_H_

#include "Base.h"
#include "Camera.h"

namespace gameplay
{

/**
 * Enables debug drawing for the physics system.
 */
class PhysicsDebugDrawer : public btIDebugDraw
{
public:
    /**
     * Constructor.
     */
	PhysicsDebugDrawer();

    /**
     * Destructor.
     */
    ~PhysicsDebugDrawer();

    /**
     * Begins drawing a frame.
     * 
     * @param viewProjection The view projection matrix to use when drawing.
     */
    void begin(const Matrix& viewProjection);

    /**
     * Finishes drawing a frame.
     */
    void end();

    // Overriden functions from btIDebugDraw.
    void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
    void reportErrorWarning(const char* warningString);
    void draw3dText(const btVector3& location, const char* textString);
    void setDebugMode(int mode);
    int	getDebugMode() const;

private:
    int _mode;
    GLuint _program;
    GLuint _positionAttrib;
    GLuint _colorAttrib;
    GLuint _viewProjectionMatrixUniform;
    const Matrix* _viewProjection;
    float* _vertexData;
    unsigned int _vertexCount;
    unsigned int _vertexDataSize;
};

}

#endif
