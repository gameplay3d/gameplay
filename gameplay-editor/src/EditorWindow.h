#pragma once

#include <QMainWindow>
#include <QToolButton>

namespace Ui {
class EditorWindow;
}
class DockWidgetManager;
class ProjectWizard;
class ProjectView;
class GameView;
class SceneView;
class PropertiesView;
class Project;

#include "gameplay.h"

/**
 * Defines the main editor window and toolbars.
 */
class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:


    /**
     * The edit mode the editor.
     */
    enum EditMode
    {
        EDIT_OBJECT
    };

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit EditorWindow(QWidget* parent = nullptr);
    
    /**
     * Destructor.
     */
    ~EditorWindow();

    /**
     * Gets the asset path for the project.
     *
     * @return The asset path.
     */
    std::string getAssetPath() const;

    /**
     * Sets the project wizard.
     *
     * @param projectWizard
     */
    void setProjectWizard(ProjectWizard* projectWizard);

    /**
     * Gets the opened project.
     *
     * @return The currently open project.
     */
    std::shared_ptr<Project> getProject() const;

    /**
     * Gets the edit mode the editor is currently in.
     *
     * @return The edit mode the editor is currently in.
     */
    EditMode getEditMode() const;

    /**
     * Gets the opened scene.
     *
     * @return The open scene.
     */
    std::shared_ptr<gameplay::Scene> getScene() const;

    /**
     * Gets the scene view.
     *
     * @return The scene view.
     */
    SceneView* getSceneView() const;

    /**
     * Gets the properties view.
     *
     * @return The properties view.
     */
    PropertiesView* getPropertiesView() const;

    /**
     * Gets the begin screen position for a selection or nullptr for no selection.
     *
     * @return The begin screen position for a selection or nullptr for no selection.
     */
    gameplay::Vector2* getSelectionBegin() const;

    /**
     * Gets the end screen position for a drag selection or nullptr for no drag selection.
     *
     * @return The screen end point for a drag selection or nullptr for no drag selection.
     */
    gameplay::Vector2* getSelectionEnd() const;

    /**
     * Determines if the editor is in fullscreen or windowed mode.
     * 
     * @return true if the editor is in fulscreen, false if windowed.
     */
    bool isFullscreen() const;

    /**
     * Set the editor in fullscreen or windowed mode.
     *
     * @param fullscreen true to set the editor to fullscreen, false for windowed mode.
     */
    void setFullscreen(bool fullscreen);

public slots:

    void onOpenProject();

    void onNewProject();

    void onOpenProject(const QString& path);

    void onOpenScene(const QString& path);

    void onTransformPan(bool checked);

    void onTransformMove(bool checked);

    void onTransformRotate(bool checked);

    void onTransformScale(bool checked);

    void onTransformModeWorld();

    void onTransformModeLocal();

    void onShadingLit();

    void onShadingUnlit();

    void onShadingWireframe();

    void onShowGrid(bool show);

    void onShowBounds(bool show);

    void onWindowProperties(bool visible);

    void onWindowScene(bool visible);

    void onWindowProject(bool visible);

    void onFullscreen();

signals:

    /**
     * Signal emitted once the window has completed restoring state
     */
    void restoringState();

    /**
     * Signal emitted when the project changes.
     */
    void projectChanged();

    /**
     * Signal emitted when the scene changes.
     */
    void sceneChanged();

    /**
     * Signal emitted when selection begin or end points change or either become deselected.
     */
    void selectionChanged();

private:
    Ui::EditorWindow* _ui;
    DockWidgetManager* _dockWidgetManager;
    GameView* _gameView;
    ProjectWizard* _projectWizard;
    ProjectView* _projectView;
    PropertiesView *_propertiesView;
    SceneView*_sceneView;
    QString _scenePath;
    std::shared_ptr<gameplay::Scene> _scene;
    EditMode _editMode;
    gameplay::Vector2* _selectionBegin;
    gameplay::Vector2* _selectionEnd;
    QToolButton* _transformModeButton;
    QToolButton* _shadingButton;
    QToolButton* _showButton;
    std::string _assetPath;
    bool _fullscreen;
    QByteArray _savedGeometry;
    QByteArray _savedState;
};
