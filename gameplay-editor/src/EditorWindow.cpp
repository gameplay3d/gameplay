#include "EditorWindow.h"
#include "DockWidgetManager.h"
#include "DockWidget.h"
#include "GameView.h"
#include "Project.h"
#include "ProjectWizard.h"
#include "ProjectTreeView.h"
#include "ProjectView.h"
#include "SceneView.h"
#include "PropertiesView.h"
#include "ui_EditorWindow.h"
#include <QtWidgets>

#define EDITOR_WINDOW_TITLE "GamePlay Editor"

EditorWindow::EditorWindow(QWidget* parent) : QMainWindow(parent), 
    _ui(new Ui::EditorWindow),
    _gameView(nullptr),
    _dockWidgetManager(nullptr),
    _projectWizard(nullptr),
    _projectView(nullptr),
    _propertiesView(nullptr),
    _sceneView(nullptr),
    _scenePath(""),
    _scene(nullptr),
    _editMode(EDIT_OBJECT),
    _selectionBegin(nullptr),
    _selectionEnd(nullptr),
    _transformModeButton(nullptr),
    _shadingButton(nullptr),
    _showButton(nullptr)
{
    _dockWidgetManager = new DockWidgetManager(this);
    _ui->setupUi(this);

    // Enable some ui effects
    QApplication::setEffectEnabled(Qt::UI_FadeMenu);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip);

    setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks);

    // Initialize transform mode toolbar buttons
    QMenu* transformModeMenu = new QMenu();
    _transformModeButton = (QToolButton*)_ui->toolBarTransform->widgetForAction(_ui->actionTransformMode);
    _transformModeButton->setPopupMode(QToolButton::InstantPopup);
    _transformModeButton->setMenu(transformModeMenu);
    _transformModeButton->setDefaultAction(_ui->actionTransformModeWorld);
    transformModeMenu->addAction(_ui->actionTransformModeWorld);
    transformModeMenu->addAction(_ui->actionTransformModeLocal);
    QActionGroup* transformGroup = new QActionGroup(this);
    transformGroup->addAction(_ui->actionTransformPan);
    transformGroup->addAction(_ui->actionTransformMove);
    transformGroup->addAction(_ui->actionTransformRotate);
    transformGroup->addAction(_ui->actionTransformScale);

    // Initialize the status bar
    QStatusBar* statusBar = this->statusBar();
    statusBar->showMessage(tr("Status: "));

    // Set the project view
    _dockWidgetManager->registerDock(_ui->dockWidgetProject);
    _projectView = new ProjectView(_ui->dockWidgetProject);
    _ui->dockWidgetProject->setWidget(_projectView);

    // Set the scene view
    _dockWidgetManager->registerDock(_ui->dockWidgetScene);
    _sceneView = new SceneView(_ui->dockWidgetScene);
    _ui->dockWidgetScene->setWidget(_sceneView);
    _sceneView->setEditor(this);

    // Set the properties view
    _dockWidgetManager->registerDock(_ui->dockWidgetProperties);
    _propertiesView = new PropertiesView(_ui->dockWidgetProperties);
    _ui->dockWidgetProperties->setWidget(_propertiesView);
    _propertiesView->setEditor(this);

    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);
    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);

    // Set the game view
    _gameView = new GameView();
    setCentralWidget(_gameView);
    _gameView->setEditor(this);

    // Set the dock corners
    setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Initialize the selection begin and end to offscreen which means no selection.
    _selectionBegin = new Vector2(-1, -1);
    _selectionEnd = new Vector2(-1, -1);

    connect(_ui->actionNewProject, SIGNAL(triggered()), this, SLOT(onNewProjectTriggered()));
    connect(_ui->actionOpenProject, SIGNAL(triggered()), this, SLOT(onOpenProjectTriggered()));
    connect(_ui->actionTransformPan, SIGNAL(triggered(bool)), this, SLOT(onTransformPanTriggered(bool)));
    connect(_ui->actionTransformMove, SIGNAL(triggered(bool)), this, SLOT(onTransformMoveTriggered(bool)));
    connect(_ui->actionTransformRotate, SIGNAL(triggered(bool)), this, SLOT(onTransformRotateTriggered(bool)));
    connect(_ui->actionTransformScale, SIGNAL(triggered(bool)), this, SLOT(onTransformScaleTriggered(bool)));
    connect(_ui->actionTransformModeWorld, SIGNAL(triggered()), this, SLOT(onTransformModeWorldTriggered()));
    connect(_ui->actionTransformModeLocal, SIGNAL(triggered()), this, SLOT(onTransformModeLocalTriggered()));
    connect(_ui->actionShadingLit, SIGNAL(triggered()), this, SLOT(onShadingLitTriggered()));
    connect(_ui->actionShadingUnlit, SIGNAL(triggered()), this, SLOT(onShadingUnlitTriggered()));
    connect(_ui->actionShadingWireframe, SIGNAL(triggered()), this, SLOT(onShadingWireframeTriggered()));
    connect(_ui->actionShowGrid, SIGNAL(triggered(bool)), this, SLOT(onShowGridTriggered(bool)));
    connect(_ui->actionShowBounds, SIGNAL(triggered(bool)), this, SLOT(onShowBoundsTriggered(bool)));
    connect(_projectView, SIGNAL(sceneOpened(QString)), this, SLOT(onOpenScene(QString)));
    connect(this, SIGNAL(sceneChanged()), _sceneView, SLOT(onSceneChanged()));
    connect(_sceneView, SIGNAL(selectionChanged()), _propertiesView, SLOT(onSelectionChanged()));
    connect(_ui->actionCreateObject, SIGNAL(triggered()), _sceneView, SLOT(onCreateObjectTriggered()));
    connect(_propertiesView, SIGNAL(nameChanged()), _sceneView, SLOT(onNameChanged()));
    connect(_ui->actionWindowProperties, SIGNAL(triggered(bool)), this, SLOT(onWindowPropertiesTriggered(bool)));
    connect(_ui->actionWindowScene, SIGNAL(triggered(bool)), this, SLOT(onWindowSceneTriggered(bool)));
    connect(_ui->actionWindowProject, SIGNAL(triggered(bool)), this, SLOT(onWindowProjectTriggered(bool)));

    emit restoringState();
}

EditorWindow::~EditorWindow()
{
    delete _ui;
}

std::string EditorWindow::getAssetPath() const
{
    return _assetPath;
}

void EditorWindow::setProjectWizard(ProjectWizard* projectWizard)
{
    _projectWizard = projectWizard;
    connect(_projectWizard, SIGNAL(projectOpened(QString)), this, SLOT(onOpenProject(QString)));
}

std::shared_ptr<Project> EditorWindow::getProject() const
{
    return _projectView->getProject();
}

 EditorWindow::EditMode EditorWindow::getEditMode() const
 {
     return _editMode;
 }

std::shared_ptr<gameplay::SceneObject> EditorWindow::getScene() const
{
    return _scene;
}

SceneView* EditorWindow::getSceneView() const
{
    return _sceneView;
}

PropertiesView* EditorWindow::getPropertiesView() const
{
    return _propertiesView;
}

gameplay::Vector2* EditorWindow::getSelectionBegin() const
{
    if (_selectionBegin->x < 0 || _selectionBegin->y < 0)
        return nullptr;

    return _selectionBegin;
}

gameplay::Vector2* EditorWindow::getSelectionEnd() const
{
    if (_selectionEnd->x < 0 || _selectionEnd->y < 0)
        return nullptr;

    return _selectionEnd;
}

void EditorWindow::onNewProjectTriggered()
{
    _projectWizard->show();
    _projectWizard->onNewProjectPressed();
}

void EditorWindow::onOpenProjectTriggered()
{
    _projectWizard->readProjects();
    _projectWizard->show();
}

void EditorWindow::onOpenProject(const QString& path)
{
    _projectView->openProject(path);
    std::shared_ptr<Project> project = _projectView->getProject();
    if (project)
    {
        setWindowTitle(QString(QLatin1String(EDITOR_WINDOW_TITLE)) +
                       QString(QLatin1String(" - ")) +
                       _projectView->getProject()->getName());
    }
}

void EditorWindow::onOpenScene(const QString& path)
{
    if (path.compare(_scenePath) != 0)
    {
        _scenePath = path;
        QByteArray pathByteArray = path.toLatin1();
        auto reader = gameplay::Serializer::createReader(pathByteArray.data());
        _scene = std::dynamic_pointer_cast<gameplay::SceneObject>(reader->readObject(nullptr));

        emit sceneChanged();
    }
}

void EditorWindow::onTransformPanTriggered(bool checked)
{
}

void EditorWindow::onTransformMoveTriggered(bool checked)
{
}

void EditorWindow::onTransformRotateTriggered(bool checked)
{
}

void EditorWindow::onTransformScaleTriggered(bool checked)
{
}


void EditorWindow::onTransformModeWorldTriggered()
{

    _transformModeButton->setDefaultAction(_ui->actionTransformModeWorld);

    // TODO
}

void EditorWindow::onTransformModeLocalTriggered()
{
    _transformModeButton->setDefaultAction(_ui->actionTransformModeLocal);
    // TODO:
}


void EditorWindow::onShadingLitTriggered()
{
    _shadingButton->setDefaultAction(_ui->actionShadingLit);
    // TODO:
}

void EditorWindow::onShadingUnlitTriggered()
{

    _shadingButton->setDefaultAction(_ui->actionShadingUnlit);
    // TODO:
}

void EditorWindow::onShadingWireframeTriggered()
{
    _shadingButton->setDefaultAction(_ui->actionShadingWireframe);
    // TODO:
}

void EditorWindow::onShowGridTriggered(bool show)
{
    // TODO
}

void EditorWindow::onShowBoundsTriggered(bool show)
{

    // TODO:
}

void EditorWindow::onWindowPropertiesTriggered(bool visible)
{
    _ui->dockWidgetProperties->show();
}

void EditorWindow::onWindowSceneTriggered(bool visible)
{
    _ui->dockWidgetScene->show();
}

void EditorWindow::onWindowProjectTriggered(bool visible)
{
    _ui->dockWidgetProject->show();
}
