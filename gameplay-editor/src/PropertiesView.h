#pragma once

#include "EditorWindow.h"
#include "SceneView.h"
#include "PropertiesComponentEditor.h"
#include "PropertiesTransform.h"
#include "PropertiesCamera.h"
#include "gameplay.h"
#include <QWidget>
#include <QFrame>


namespace Ui
{
class PropertiesView;
class PropertiesEditor;
class PropertiesTransform;
class PropertiesCamera;
class PropertiesLight;
}

/**
 * Defines a view for editing properties associated to a scene object.
 */
class PropertiesView : public QWidget
{
    Q_OBJECT
public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~PropertiesView();

    /**
     * Sets the editor this scene view is connected to.
     *
     * @param editor The editor this scene view is connected to.
     */
    void setEditor(EditorWindow* editor);

    /**
     * Gets the name of the current property being edited.
     *
     * @return The name of the current property being edited.
     */
    QString getName() const;

signals:

    void nameChanged();

public slots:

    /**
     * Handler for when an object selected in the scene has changed.
     */
    void onSelectionChanged();

    /**
     * Handler for when the selected objects name changes.
     */
    void onNameChanged();

    /**
     * Handler for when an editor has been resized.
     */
    void onEditorsResized();

private:

    void hideProperties();
    void showProperties();
    void clearPropertyEditors();


    Ui::PropertiesView* _ui;
    EditorWindow* _editor;
    SceneView* _sceneView;
    gameplay::SceneObject _selectedObject;
    std::vector<std::shared_ptr<gameplay::Component>> _components;
};
