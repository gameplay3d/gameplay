#include "PropertiesTransform.h"
#include "ui_PropertiesTransform.h"

PropertiesTransform::PropertiesTransform(QWidget* parent) :
    QFrame(parent),
    _ui(new Ui::PropertiesTransform)
{
    _ui->setupUi(this);

    QButtonGroup* buttonGroupMobility = new QButtonGroup(this);
    buttonGroupMobility->addButton(_ui->pushButtonStatic);
    buttonGroupMobility->addButton(_ui->pushButtonMovable);
}

PropertiesTransform::~PropertiesTransform()
{
    delete _ui;
}
