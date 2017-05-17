#include "SceneView.h"
#include "ui_SceneView.h"
#include "PropertiesView.h"
#include <QMenu>
#include <QItemSelectionRange>

#define SCENE_OBJECT_NAME "object"
#define SCENE_EXT ".scene"

SceneView::SceneView(QWidget* parent) : QWidget(parent),
    _ui(new Ui::SceneView),
    _editor(nullptr),
    _scene(nullptr),
    _sceneModel(nullptr),
    _sortFilter(nullptr),
    _selectedItems(nullptr)
{
    _ui->setupUi(this);

    _ui->lineEditSearch->addAction(QIcon(":/res/images/search.png"), QLineEdit::LeadingPosition);

    _sceneModel = new QStandardItemModel();
    // Sort and search filter
    _sortFilter = new SceneSortFilterProxyModel();
    _sortFilter->setDynamicSortFilter(true);
    _sortFilter->setFilterKeyColumn(0);
    _sortFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _sortFilter->setSourceModel(_sceneModel);

    _selectedItems = new std::list<QStandardItem*>();

    _ui->treeView->setUniformRowHeights(true);
    _ui->treeView->setSortingEnabled(false);
    _ui->treeView->setModel(_sortFilter);

    connect(_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(onSearchTextChanged(QString)));
    connect(_sceneModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onModelDataChanged(QModelIndex, QModelIndex)));
    connect(_ui->treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(onModelSelectionChanged(QItemSelection, QItemSelection)));
}

SceneView::~SceneView()
{
    delete _ui;
    delete _sceneModel;
    _selectedItems->clear();
    delete _selectedItems;
}

void SceneView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

std::list<QStandardItem*>* SceneView::getSelectedItems() const
{
    return _selectedItems;
}

void SceneView::onSceneChanged()
{
    _scene = _editor->getScene();
    QStandardItem* item = createHierarchy(_scene);
    item->setEditable(false);
    QString text = item->text().append(SCENE_EXT);
    item->setText(text);
    item->setIcon(QIcon(":/res/images/scene-hierarchy.png"));
    _sceneModel->appendRow(item);
    _ui->treeView->setExpanded(_sortFilter->mapFromSource(item->index()), true);
    _ui->treeView->setItemsExpandable(false);
}

void SceneView::onEditorSelectionChanged()
{
    gameplay::Vector2* selectionBegin = _editor->getSelectionBegin();
    gameplay::Vector2* selectedEnd = _editor->getSelectionEnd();

    // TODO
    // Project the point or region against all the objects in the scene
    // updating the selected objects list.

}

void SceneView::onModelSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    for (size_t i = 0; i < deselected.size(); ++i)
    {
        QModelIndex index = deselected.at(i).topLeft();
        QStandardItem* item = _sceneModel->itemFromIndex(_sortFilter->mapToSource(index));
        _selectedItems->remove(item);
    }
    for (size_t i = 0; i < selected.size(); ++i)
    {
        QModelIndex index = selected.at(i).topLeft();
        QStandardItem* item = _sceneModel->itemFromIndex(_sortFilter->mapToSource(index));
        _selectedItems->push_back(item);
    }

    emit selectionChanged();
}

void SceneView::onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    QVariant userData = topLeft.data(Qt::UserRole + 1);
    gameplay::SceneObject* object = (gameplay::SceneObject*)userData.toLongLong();
    QVariant displayData = topLeft.data(Qt::DisplayRole);
    object->setName(displayData.toString().toLatin1().constData());

    emit selectionChanged();
}

QStandardItem* SceneView::createItem(std::shared_ptr<gameplay::SceneObject> object)
{
    QString text;
    std::string name = object->getName();
    if (name.length() > 0)
        text = QString(name.c_str());
    else
        text = QString(tr(SCENE_OBJECT_NAME));

    QStandardItem* item = new QStandardItem(QIcon(":/res/images/scene-object.png"), text);
    item->setEditable(true);
    // Associate the object to the item
    item->setData(QVariant::fromValue((qlonglong)object.get()), Qt::UserRole + 1);

    return item;
}

QStandardItem* SceneView::createHierarchy(std::shared_ptr<gameplay::SceneObject> object)
{
    // Clone the object into a item for the model
    QStandardItem* item = createItem(object);
    // Visit all the children adding items for each object
    visitorAddItem(object, item);

    return item;
}

void SceneView::visitorAddItem(std::shared_ptr<gameplay::SceneObject> parent, QStandardItem* parentItem)
{
    auto children = parent->getChildren();
    for (auto object : children)
    {
        QStandardItem* item = createHierarchy(object);
        parentItem->appendRow(item);
    }
}

void SceneView::addToHiearchy(std::shared_ptr<gameplay::SceneObject> object, QStandardItem* item)
{
    // If there is no object selected the just add to the scene
    if (_selectedItems->size() == 0)
    {
        _sceneModel->appendRow(item);
        _scene->addChild(object);
    }
    else
    {
        // If any objects are selected then add to the first selected and expand the parent
        QStandardItem* selectedItem = _selectedItems->front();
        selectedItem->appendRow(item);
        _ui->treeView->setExpanded(_sortFilter->mapFromSource(selectedItem->index()), true);
        QVariant userData = selectedItem->data(Qt::UserRole + 1);
        gameplay::SceneObject* selectedObject = (gameplay::SceneObject*)userData.toLongLong();
        selectedObject->addChild(object);
    }
    _ui->treeView->selectionModel()->setCurrentIndex(_sortFilter->mapFromSource(item->index()),
                                                     QItemSelectionModel::ClearAndSelect);
}

void SceneView::onSearchTextChanged(const QString& text)
{
    _sortFilter->setFilterRegExp(text);
}

void SceneView::onCreateObject()
{
    // Create an empty object
    auto object = std::make_shared<gameplay::SceneObject>();
    object->setName(SCENE_OBJECT_NAME);
    // Create an empty item.
    QStandardItem* item = createItem(object);
    // Add the object to the scene hierarchy
    addToHiearchy(object, item);
}

void SceneView::onNameChanged()
{
    if (_selectedItems->size() == 1)
    {
        QStandardItem* item = _selectedItems->front();
        PropertiesView* propertiesView = _editor->getPropertiesView();
        QString name = propertiesView->getName();

        item->setText(name);
        QVariant userData = item->data(Qt::UserRole + 1);
        gameplay::SceneObject* selectedObject = (gameplay::SceneObject*)userData.toLongLong();
        selectedObject->setName(name.toStdString().c_str());
    }
}
