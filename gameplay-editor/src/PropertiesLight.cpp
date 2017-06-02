#include "PropertiesLight.h"
#include "ui_PropertiesLight.h"
#include <QColorDialog>

#define PROPERTIES_LIGHT_DIRECTION_HEIGHT 160
#define PROPERTIES_LIGHT_POINT_HEIGHT 190
#define PROPERTIES_LIGHT_SPOT_HEIGHT 220

PropertiesLight::PropertiesLight(QWidget* parent) : QWidget(parent),
    _ui(new Ui::PropertiesLight),
    _light(nullptr)
{
    _ui->setupUi(this);

    setColor(QColor::fromRgb(1, 1, 1));

    connect(_ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
    connect(_ui->comboBoxLighting, SIGNAL(currentIndexChanged(int)), this, SLOT(onLightingChanged(int)));
    connect(_ui->comboBoxShadows, SIGNAL(currentIndexChanged(int)), this, SLOT(onShadowsChanged(int)));
    connect(_ui->toolButtonColor, SIGNAL(pressed()), this, SLOT(onColorPressed()));
    connect(_ui->horizontalSliderAngle, SIGNAL(valueChanged(int)), this, SLOT(onAngleSliderChanged(int)));
    connect(_ui->spinBoxAngle, SIGNAL(valueChanged(int)), this, SLOT(onAngleValueChanged(int)));
}

PropertiesLight::~PropertiesLight()
{
    delete _ui;
}

void PropertiesLight::setLight(std::shared_ptr<gameplay::Light> light)
{
    _light = light;

    onTypeChanged(0);
    onLightingChanged(0);
    onShadowsChanged(0);
}

QString PropertiesLight::getHeaderName() const
{
    return QString("Light");
}

QString PropertiesLight::getHeaderIcon() const
{
    return QString(":/res/images/scene-light-point.png");
}

//160, 190, 220
void PropertiesLight::onTypeChanged(int index)
{
    switch (index)
    {
    case gameplay::Light::TYPE_DIRECTIONAL:
    {
        _ui->labelRange->hide();
        _ui->doubleSpinBoxRange->hide();
        _ui->labelAngle->hide();
        _ui->horizontalSliderAngle->hide();
        _ui->spinBoxAngle->hide();
        setGeometry(geometry().x(), geometry().y(), geometry().width(), PROPERTIES_LIGHT_DIRECTION_HEIGHT);
        break;
    }

    case gameplay::Light::TYPE_POINT:
    {
        _ui->labelRange->show();
        _ui->doubleSpinBoxRange->show();
        _ui->labelAngle->hide();
        _ui->horizontalSliderAngle->hide();
        _ui->spinBoxAngle->hide();
        setGeometry(geometry().x(), geometry().y(), geometry().width(), PROPERTIES_LIGHT_POINT_HEIGHT);
        break;
    }

    case gameplay::Light::TYPE_SPOT:
    {
        _ui->labelRange->show();
        _ui->doubleSpinBoxRange->show();
        _ui->labelAngle->show();
        _ui->horizontalSliderAngle->show();
        _ui->spinBoxAngle->show();
        setGeometry(geometry().x(), geometry().y(), geometry().width(), PROPERTIES_LIGHT_SPOT_HEIGHT);
    }

    default:
        break;
    }
    _light->setType(static_cast<gameplay::Light::Type>(index));
}

void PropertiesLight::onReset(bool)
{
    _light->reset(static_cast<gameplay::Light::Type>(_ui->comboBoxType->currentIndex()));
    updateProperties();
}

void PropertiesLight::onLightingChanged(int index)
{
    _light->setLighting(static_cast<gameplay::Light::Lighting>(index));
}

void PropertiesLight::onShadowsChanged(int index)
{
     _light->setShadows(static_cast<gameplay::Light::Shadows>(index));
}

void PropertiesLight::onColorPressed()
{
    QColor color = QColorDialog::getColor();
    setColor(color);
}

void PropertiesLight::onAngleSliderChanged(int value)
{
    _ui->spinBoxAngle->setValue(value);
}

void PropertiesLight::onAngleValueChanged(int value)
{
    _ui->horizontalSliderAngle->setValue(value);
}

void PropertiesLight::updateProperties()
{
}

void PropertiesLight::setColor(QColor color)
{
    QPixmap pixmap(160, 16);
    pixmap.fill(color);
    _ui->toolButtonColor->setIcon(pixmap);
}
