#ifndef TEXTURECUBESAMPLE_H_
#define TEXTURECUBESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample the creating and drawing a texture cube with render targets.
 */
class TextureCubeSample : public Sample
{
public:

    TextureCubeSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool drawScene(Node* node, Node* ignore);
	bool initMaterials(Node* node, Properties* prop);
	void createCubeMap();

	Font* _font;
	Scene* _scene;
	Node* _cubeCamera;
	FrameBuffer* _fBuffer;
	RenderTarget** _rTargets;
};

#endif
