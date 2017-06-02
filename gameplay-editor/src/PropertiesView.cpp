#include "PropertiesView.h"
#include "ui_PropertiesView.h"
#include "ui_PropertiesComponentEditor.h"
#include "ui_PropertiesTransform.h"
#include <QMenu>

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

    hideProperties();
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
        showProperties();

        // Object enabled + name
        _ui->checkBoxEnabled->setChecked(selectedObject->isEnabled());
        _ui->lineEditName->setText(QString(selectedObject->getName().c_str()));

        // Set the transform
        _ui->propertiesTransform->setObject(selectedObject);
        connect(_ui->propertiesTransform, SIGNAL(opened()), this, SLOT(onEditorsResized()));

        // Clear all the existing component property editors
        clearPropertyEditors();

        // Get the attached components
        std::vector<std::shared_ptr<gameplay::Component>> components;
        selectedObject->getComponents(components);
        for (auto component : components)
        {
            // Create a property editor for each type of component
            PropertiesComponentEditor* editor = new PropertiesComponentEditor(this);
            editor->setComponent(component);
            _ui->propertyEditors->layout()->addWidget(editor);
            connect(editor, SIGNAL(opened()), this, SLOT(onEditorsResized()));
        }
    }
    else
    {
        hideProperties();
    }
}

void PropertiesView::onNameChanged()
{
    emit nameChanged();
}

void PropertiesView::onEditorsResized()
{
    _ui->propertiesTransform->updateGeometry();
}

QString PropertiesView::getName() const
{
    return _ui->lineEditName->text();
}

void PropertiesView::hideProperties()
{
    _ui->scrollAreaProperties->hide();
}

void PropertiesView::showProperties()
{
    _ui->scrollAreaProperties->show();
}

void PropertiesView::clearPropertyEditors()
{
    QLayout* layout = _ui->propertyEditors->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
    {
      delete item->widget();
      delete item;
    }
}
