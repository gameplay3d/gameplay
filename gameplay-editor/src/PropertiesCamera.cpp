#include "PropertiesCamera.h"
#include "ui_PropertiesCamera.h"
#include <QToolButton>
#include <QColorDialog>
#include <QColor>

PropertiesCamera::PropertiesCamera(QWidget* parent) : QWidget(parent),
    _ui(new Ui::PropertiesCamera),
    _camera(nullptr)
{
    _ui->setupUi(this);

    setColor(QColor::fromRgb(0, 0, 0));

    connect(_ui->comboBoxMode, SIGNAL(currentIndexChanged(int)), this, SLOT(onModeChanged(int)));
    connect(_ui->toolButtonColor, SIGNAL(pressed()), this, SLOT(onColorPressed()));
    connect(_ui->horizontalSliderFieldOfView, SIGNAL(valueChanged(int)), this, SLOT(onFieldOfViewSliderChanged(int)));
    connect(_ui->spinBoxFieldOfView, SIGNAL(valueChanged(int)), this, SLOT(onFieldOfViewValueChanged(int)));
}

PropertiesCamera::~PropertiesCamera()
{
    delete _ui;
}

void PropertiesCamera::setCamera(std::shared_ptr<gameplay::Camera> camera)
{
    _camera = camera;

    onModeChanged(0);
}

QString PropertiesCamera::getHeaderName() const
{
    return QString("Camera");
}

QString PropertiesCamera::getHeaderIcon() const
{
    return QString(":/res/images/scene-camera.png");
}

void PropertiesCamera::onReset(bool)
{
    _camera->reset(static_cast<gameplay::Camera::Mode>(_ui->comboBoxMode->currentIndex()));
    updateProperties();
}

void PropertiesCamera::onModeChanged(int index)
{
    switch (static_cast<gameplay::Camera::Mode>(index))
    {
    case gameplay::Camera::Mode::ePerspective:
        {
            _ui->labelSize->hide();
            _ui->doubleSpinBoxSize->hide();
            _ui->labelFieldOfView->show();
            _ui->spinBoxFieldOfView->show();
            _ui->horizontalSliderFieldOfView->show();
            break;
        }

    case gameplay::Camera::Mode::eOrthograhic:
        {
            _ui->labelFieldOfView->hide();
            _ui->spinBoxFieldOfView->hide();
            _ui->horizontalSliderFieldOfView->hide();
            _ui->labelSize->show();
            _ui->doubleSpinBoxSize->show();
            break;
        }

    default:
        break;
    }
}

void PropertiesCamera::onColorPressed()
{
    QColor color = QColorDialog::getColor();
    setColor(color);
}

void PropertiesCamera::setColor(QColor color)
{
    QPixmap pixmap(160, 16);
    pixmap.fill(color);
    _ui->toolButtonColor->setIcon(pixmap);
}

void PropertiesCamera::onFieldOfViewSliderChanged(int value)
{
    _ui->spinBoxFieldOfView->setValue(value);
}

void PropertiesCamera::onFieldOfViewValueChanged(int value)
{
    _ui->horizontalSliderFieldOfView->setValue(value);
}

void PropertiesCamera::updateProperties()
{

}
