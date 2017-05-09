#pragma once

#include "EditorWindow.h"
#include "SceneView.h"
#include "PropertiesTransform.h"
#include <QWidget>
#include <QFrame>
#include "gameplay.h"

using namespace gameplay;

namespace Ui {
class PropertiesView;
class PropertiesTransform;
}


/**
 * Defines a view for diplaying and editing scene objects.
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
     * Handler when the object selected in the scene has changed.
     */
    void onSelectionChanged();

    void onNameChanged();

private:

    void hideAll();
    void showAll();

    Ui::PropertiesView* _ui;
    EditorWindow* _editor;
    SceneView* _sceneView;
    std::list<QFrame*> _componentProperties;
};
