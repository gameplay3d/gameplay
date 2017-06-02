#include "PropertiesTransform.h"
#include "ui_PropertiesTransform.h"
#include <QMenu>

#define PROPERTIES_TRANSFORM_HEIGHT_CLOSED 26

PropertiesTransform::PropertiesTransform(QWidget* parent) : QWidget(parent),
    _ui(new Ui::PropertiesTransform),
    _object(nullptr),
    _expanded(true),
    _expandedHeight(0)
{
    _ui->setupUi(this);

    // Setup the mobility button group
    QButtonGroup* buttonGroupMobility = new QButtonGroup(this);
    buttonGroupMobility->addButton(_ui->pushButtonStatic);
    buttonGroupMobility->addButton(_ui->pushButtonMovable);

    // Enable the options menu
    QMenu* optionsMenu = new QMenu(this);
    optionsMenu->addAction(_ui->actionReset);
    optionsMenu->addSeparator();
    optionsMenu->addAction(_ui->actionInfo);
    _ui->toolButtonHeaderOptions->setMenu(optionsMenu);

    connect(_ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(onReset(bool)));
    connect(_ui->pushButtonHeader, SIGNAL(pressed()), this, SLOT(onHeaderPressed()));
    connect(_ui->toolButtonHeaderExpander, SIGNAL(pressed()), this, SLOT(onHeaderPressed()));
}

PropertiesTransform::~PropertiesTransform()
{
    delete _ui;
}

void PropertiesTransform::setObject(gameplay::SceneObject* object)
{
    _object = object;

    updateProperties();
    _expandedHeight = geometry().height();
}

void PropertiesTransform::updateProperties()
{
    // Object position
    const gameplay::Vector3 position = _object->getPosition();
    _ui->doubleSpinBoxPositionX->setValue(position.x);
    _ui->doubleSpinBoxPositionY->setValue(position.y);
    _ui->doubleSpinBoxPositionZ->setValue(position.z);

    // Object eulerAngles
    const gameplay::Vector3 eulerAngles = _object->getEulerAngles();
    _ui->doubleSpinBoxRotationX->setValue(eulerAngles.x);
    _ui->doubleSpinBoxRotationY->setValue(eulerAngles.y);
    _ui->doubleSpinBoxRotationZ->setValue(eulerAngles.z);

    // Object scale
    const gameplay::Vector3 localScale = _object->getLocalScale();
    _ui->doubleSpinBoxScaleX->setValue(localScale.x);
    _ui->doubleSpinBoxScaleY->setValue(localScale.y);
    _ui->doubleSpinBoxScaleZ->setValue(localScale.z);

    // Mobility
    if (_object->isStatic())
    {
        _ui->pushButtonStatic->setChecked(true);
    }
    else
    {
        _ui->pushButtonMovable->setChecked(true);
    }
}

void PropertiesTransform::onReset(bool)
{
    _object->resetLocalTransform();
    _object->setStatic(true);
    updateProperties();
}


 void PropertiesTransform::onHeaderPressed()
 {
    _expanded = !_expanded;
    if (_expanded)
    {
        setGeometry(geometry().x(), geometry().y(), geometry().width(), _expandedHeight);
        emit opened();
    }
    else
    {
        setGeometry(geometry().x(), geometry().y(), geometry().width(), PROPERTIES_TRANSFORM_HEIGHT_CLOSED);
        emit closed();
    }
 }
