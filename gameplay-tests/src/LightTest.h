#ifndef LIGHTTEST_H_
#define LIGHTTEST_H_

#include "gameplay.h"
#include "Test.h"
#include "Material.h"
#include "Model.h"

using namespace gameplay;

/**
 * Test for lights.
 */
class LightTest : public Test, Control::Listener
{
public:

    LightTest();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

	void controlEvent(Control* control, EventType evt);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:
	void initDirectionalTechniqueCommonParams(const char* technique);
	void initSpotTechniqueCommonParams(const char* technique);
	void initPointTechniqueCommonParams(const char* technique);
	void setColorValue(const Vector3& value);
	void setSpecularValue(float);

    bool drawScene(Node* node);

    Font* _font;
    Scene* _scene;
	Node* _modelNode;

    Node* _directionalLightNode;
	Node* _pointLightNode;
	Node* _spotLightNode;
	Node* _usedForMoving;

	Model* _model;
	Model* _directionalLightQuadModel;
	Model* _spotLightQuadModel;
	Model* _pointLightQuadModel;

	Material* _unlitMaterial;
	Material* _texturedMaterial;
	Material* _bumpedMaterial;
	Material* _bumpedSpecularMaterial;

	Material* _lighting;
	
	Slider* _redSlider;
	Slider* _greenSlider;
	Slider* _blueSlider;
	Slider* _specularSlider;

	RadioButton* _noLight;
	RadioButton* _directional;
	RadioButton* _spot;
	RadioButton* _point;

	CheckBox* _addSpecular;
	CheckBox* _addBumped;

	Form* _form;
    
	bool _touched;
	int _touchX, _touchY;
};

#endif
