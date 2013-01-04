#ifndef POSTPROCESSTEST_H_
#define POSTPROCESSTEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;


/**
 * Test post processing.
 */
class PostProcessTest: public Test
{
public:

	/**
	 * Constructor.
	 */
    PostProcessTest();

	/**
	 * @see Test::touchEvent
	 */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Compositing blitter with a specified material/technique applied from a source buffer into the destination buffer.
     *
     * If destination buffer is NULL then it composites to the default frame buffer.
     *
     * Requried uniforms:
     * sampler2d u_texture - The input texture sampler
     */
    class Compositor
    {
    public:
		
        static Compositor* create(FrameBuffer* srcBuffer, FrameBuffer* dstBuffer, const char* materialPath, const char* techniqueId);

        ~Compositor();

        const char* getTechniqueId() const;

        Material* getMaterial() const;

        void blit(const Rectangle& dst, bool clearBuffer, const Vector4& clearColor = Vector4::zero());

    private:

        Compositor();

        Compositor(FrameBuffer* srcBuffer, FrameBuffer* dstBuffer, Material* material, const char* techniqueId);

        FrameBuffer* _srcBuffer;
        FrameBuffer* _dstBuffer;
        Material* _material;
        const char* _techniqueId;
    };

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool drawScene(Node* node);

    void drawTechniqueId(const char* techniqueId);

private:

    Font* _font;
    Scene* _scene;
    Node* _modelNode;
    FrameBuffer* _frameBuffer;
    unsigned int _compositorIndex;
    std::vector<Compositor*> _compositors;
};

#endif
