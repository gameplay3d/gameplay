#include "PropertiesComponentEditor.h"
#include "ui_PropertiesComponentEditor.h"
#include "PropertiesCamera.h"
#include "ui_PropertiesCamera.h"
#include "PropertiesLight.h"
#include "ui_PropertiesLight.h"
#include <QMenu>

#define PROPERTIES_COMPONENT_EDITOR_HEIGHT_CLOSED 26

PropertiesComponentEditor::PropertiesComponentEditor(QWidget* parent) : QWidget(parent),
    _ui(new Ui::PropertiesComponentEditor),
    _component(nullptr),
    _expanded(true),
    _expandedHeight(0)
{
    _ui->setupUi(this);

    // Enable the options menu
    QMenu* optionsMenu = new QMenu(this);
    optionsMenu->addAction(_ui->actionReset);
    optionsMenu->addSeparator();
    optionsMenu->addAction(_ui->actionDetach);
    optionsMenu->addAction(_ui->actionInfo);
    _ui->toolButtonHeaderOptions->setMenu(optionsMenu);

    connect(_ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(onReset(bool)));
    connect(_ui->actionDetach, SIGNAL(triggered(bool)), this, SLOT(onDetach(bool)));
    connect(_ui->pushButtonHeader, SIGNAL(pressed()), this, SLOT(onHeaderPressed()));
    connect(_ui->toolButtonHeaderExpander, SIGNAL(pressed()), this, SLOT(onHeaderPressed()));
}

PropertiesComponentEditor::~PropertiesComponentEditor()
{
    delete _ui;
}

void PropertiesComponentEditor::setComponent(std::shared_ptr<gameplay::Component> component)
{
    GP_ASSERT(component);

    _ui->checkBoxEnabled->setEnabled(component->isEnabled());

    switch (component->getTypeId())
    {
        case gameplay::Component::TYPEID_CAMERA:
        {
            PropertiesCamera* propertiesCamera = new PropertiesCamera(this);
            _ui->labelHeaderName->setText(propertiesCamera->getHeaderName());
            _ui->labelHeaderIcon->setPixmap(QPixmap(propertiesCamera->getHeaderIcon()));
            _ui->widgetComponentProperties->layout()->addWidget(propertiesCamera);
            propertiesCamera->setCamera(std::dynamic_pointer_cast<gameplay::Camera>(component));
            break;
        }
        case gameplay::Component::TYPEID_LIGHT:
        {
            PropertiesLight* propertiesLight = new PropertiesLight(this);
            _ui->labelHeaderName->setText(propertiesLight->getHeaderName());
            _ui->labelHeaderIcon->setPixmap(QPixmap(propertiesLight->getHeaderIcon()));
            _ui->widgetComponentProperties->layout()->addWidget(propertiesLight);
            propertiesLight->setLight(std::dynamic_pointer_cast<gameplay::Light>(component));
            break;
        }

        default:
            break;
    }
    _expandedHeight = geometry().height();
}

void PropertiesComponentEditor::onReset(bool)
{
    // TODO
}

void PropertiesComponentEditor::onDetach(bool)
{
    // TODO
}

void PropertiesComponentEditor::onHeaderPressed()
{
   if (_expanded)
   {
       setGeometry(geometry().x(), geometry().y(), geometry().width(), PROPERTIES_COMPONENT_EDITOR_HEIGHT_CLOSED);
       emit closed();
   }
   else
   {
       setGeometry(geometry().x(), geometry().y(), geometry().width(), _expandedHeight);
       emit opened();
   }
   _expanded = !_expanded;
}



