#ifndef LIGHTSAMPLE_H_
#define LIGHTSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"
#include "Material.h"
#include "Model.h"

using namespace gameplay;

/**
 * Sample for lights.
 */
class LightSample : public Sample, Control::Listener
{
public:

    LightSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

	void controlEvent(Control* control, EventType evt);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

	void initializeDirectionalTechnique(const char* technique);

	void initializeSpotTechnique(const char* technique);

	void initializePointTechnique(const char* technique);

    void setUnlitMaterialTexture(Model* model, const char* texturePath, bool mipmap = true);

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
	
	RadioButton* _noLight;
	RadioButton* _directional;
	RadioButton* _spot;
	RadioButton* _point;

    Container* _properties;
	Slider* _redSlider;
	Slider* _greenSlider;
	Slider* _blueSlider;
	Slider* _specularSlider;
    CheckBox* _addSpecular;
	CheckBox* _addBumped;

	Form* _form;
    
	bool _touched;
	int _touchX, _touchY;
};

#endif
