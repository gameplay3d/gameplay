#include "PropertiesView.h"
#include "ui_PropertiesView.h"
#include "ui_PropertiesTransform.h"
#include <QMenu>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QSpacerItem>

PropertiesView::PropertiesView(QWidget* parent) : QWidget(),
    _ui(new Ui::PropertiesView),
    _editor(nullptr),
    _sceneView(nullptr)
{
    _ui->setupUi(this);

    // Create an attach components menu
    QMenu* attachComponentMenu = new QMenu(this);
    attachComponentMenu->addAction(_ui->actionAttachCamera);
    attachComponentMenu->addAction(_ui->actionAttachLight);
    _ui->pushButtonAttachComponent->setMenu(attachComponentMenu);

    connect(_ui->lineEditName, SIGNAL(textChanged(QString)), this, SLOT(onNameChanged()));

    hideAll();
}

PropertiesView::~PropertiesView()
{
    delete _ui;
}

void PropertiesView::setEditor(EditorWindow* editor)
{
    _editor = editor;
    _sceneView = _editor->getSceneView();
}

void PropertiesView::onSelectionChanged()
{
    std::list<QStandardItem*>* selectedItems = _editor->getSceneView()->getSelectedItems();
    if (selectedItems->size() == 0 || selectedItems->size() > 1)
        return;

    QStandardItem* selectedItem = selectedItems->front();
    QVariant userData = selectedItem->data(Qt::UserRole + 1);
    gameplay::SceneObject* selectedObject = (gameplay::SceneObject*)userData.toLongLong();

    if (selectedObject != _editor->getScene().get())
    {
        showAll();

        // Object enabled + name
        _ui->checkBoxEnabled->setChecked(selectedObject->isEnabled());
        _ui->lineEditName->setText(QString(selectedObject->getName().c_str()));

        // Object position
        const Vector3 position = selectedObject->getPosition();
        _ui->frameTransform->_ui->doubleSpinBoxPositionX->setValue(position.x);
        _ui->frameTransform->_ui->doubleSpinBoxPositionY->setValue(position.y);
        _ui->frameTransform->_ui->doubleSpinBoxPositionZ->setValue(position.z);

        // Object rotation (eulerAngles)
        const Vector3 eulerAngles = selectedObject->getEulerAngles();
        _ui->frameTransform->_ui->doubleSpinBoxRotationX->setValue(eulerAngles.x);
        _ui->frameTransform->_ui->doubleSpinBoxRotationY->setValue(eulerAngles.y);
        _ui->frameTransform->_ui->doubleSpinBoxRotationZ->setValue(eulerAngles.z);

        // Object scale
        const Vector3 localScale = selectedObject->getLocalScale();
        _ui->frameTransform->_ui->doubleSpinBoxScaleX->setValue(localScale.x);
        _ui->frameTransform->_ui->doubleSpinBoxScaleY->setValue(localScale.y);
        _ui->frameTransform->_ui->doubleSpinBoxScaleZ->setValue(localScale.z);

        // Mobility
        if (selectedObject->isStatic())
        {
            _ui->frameTransform->_ui->pushButtonStatic->setChecked(true);
        }
        else
        {
            _ui->frameTransform->_ui->pushButtonMovable->setChecked(true);
        }

        // Remove the previous components
        for (QFrame* frame : _componentProperties)
        {

        }

        // Add the attached components

    }
    else
    {
        hideAll();
    }
}

void PropertiesView::onNameChanged()
{
    emit nameChanged();
}

QString PropertiesView::getName() const
{
    return _ui->lineEditName->text();
}

void PropertiesView::hideAll()
{
    _ui->checkBoxEnabled->hide();
    _ui->lineEditName->hide();
    _ui->pushButtonAttachComponent->hide();
    _ui->frameTransform->hide();
    _ui->scrollAreaProperties->hide();
}

void PropertiesView::showAll()
{
    _ui->checkBoxEnabled->show();
    _ui->lineEditName->show();
    _ui->pushButtonAttachComponent->show();
    _ui->frameTransform->show();
    _ui->scrollAreaProperties->show();
}


