#pragma once

#include "EditorWindow.h"
#include "SceneSortFilterProxyModel.h"
#include "gameplay.h"
#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui
{
class SceneView;
}


/**
 * Defines the scene view to display the hierarchy of scene.
 */
class SceneView : public QWidget
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit SceneView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~SceneView();

    /**
     * Sets the editor this scene view is connected to.
     *
     * @param editor The editor this scene view is connected to.
     */
    void setEditor(EditorWindow* editor);

    /**
     * Gets the list of selected items in the scene.
     *
     * @return The list of selected items in the scene.
     */
    std::list<QStandardItem*>* getSelectedItems() const;

public slots:

    /**
     * Handler when the scene changes.
     */
    void onSceneChanged();

    /**
     * Handler for when the editor selection points change.
     */
    void onEditorSelectionChanged();

    /**
     * @see QItemSelectionModel::selectionChanged
     */
    void onModelSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    /**
     * @see QAbstractItemModel::dataChanged
     */
    void onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    /**
     * Handler for when the search filter text changes.
     *
     * @param text The changed text.
     */
    void onSearchTextChanged(const QString& text);

    /**
     * Handlers for when an action to add a object is triggered.
     */
    void onCreateObject();

    /**
     * Handles when the selected object(s) name changes
     */
    void onNameChanged();

signals:

    /**
     * Signal emitted when selection begin or end points change or either become deselected.
     */
    void selectionChanged();
    
private:

    QStandardItem* createItem(std::shared_ptr<gameplay::SceneObject> object);
    QStandardItem* createHierarchy(std::shared_ptr<gameplay::SceneObject> object);
    void visitorAddItem(std::shared_ptr<gameplay::SceneObject> parent, QStandardItem* parentItem);
    void addToHiearchy(std::shared_ptr<gameplay::SceneObject> object, QStandardItem* item);

    Ui::SceneView* _ui;
    EditorWindow* _editor;
    std::shared_ptr<gameplay::SceneObject> _scene;
    QStandardItemModel* _sceneModel;
    SceneSortFilterProxyModel* _sortFilter;
    std::list<QStandardItem*>* _selectedItems;
};
