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

#define TYPE_WINDOW_TITLE "GamePlay Editor"

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
    _showButton(nullptr),
    _fullscreen(false)
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
    _gameView = _ui->gameView;
    _gameView->setEditor(this);

    // Set the dock corners
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    connect(_ui->actionNewProject, SIGNAL(triggered()), this, SLOT(onNewProject()));
    connect(_ui->actionOpenProject, SIGNAL(triggered()), this, SLOT(onOpenProject()));
    connect(_ui->actionTransformPan, SIGNAL(triggered(bool)), this, SLOT(onTransformPan(bool)));
    connect(_ui->actionTransformMove, SIGNAL(triggered(bool)), this, SLOT(onTransformMove(bool)));
    connect(_ui->actionTransformRotate, SIGNAL(triggered(bool)), this, SLOT(onTransformRotate(bool)));
    connect(_ui->actionTransformScale, SIGNAL(triggered(bool)), this, SLOT(onTransformScale(bool)));
    connect(_ui->actionTransformModeWorld, SIGNAL(triggered()), this, SLOT(onTransformModeWorld()));
    connect(_ui->actionTransformModeLocal, SIGNAL(triggered()), this, SLOT(onTransformModeLocal()));
    connect(_ui->actionShadingLit, SIGNAL(triggered()), this, SLOT(onShadingLit()));
    connect(_ui->actionShadingUnlit, SIGNAL(triggered()), this, SLOT(onShadingUnlit()));
    connect(_ui->actionShadingWireframe, SIGNAL(triggered()), this, SLOT(onShadingWireframe()));
    connect(_ui->actionShowGrid, SIGNAL(triggered(bool)), this, SLOT(onShowGrid(bool)));
    connect(_ui->actionShowBounds, SIGNAL(triggered(bool)), this, SLOT(onShowBounds(bool)));
    connect(_projectView, SIGNAL(sceneOpened(QString)), this, SLOT(onOpenScene(QString)));
    connect(this, SIGNAL(sceneChanged()), _sceneView, SLOT(onSceneChanged()));
    connect(_sceneView, SIGNAL(selectionChanged()), _propertiesView, SLOT(onSelectionChanged()));
    connect(_ui->actionCreateObject, SIGNAL(triggered()), _sceneView, SLOT(onCreateObject()));
    connect(_propertiesView, SIGNAL(nameChanged()), _sceneView, SLOT(onNameChanged()));
    connect(_ui->actionWindowProperties, SIGNAL(triggered(bool)), this, SLOT(onWindowProperties(bool)));
    connect(_ui->actionWindowScene, SIGNAL(triggered(bool)), this, SLOT(onWindowScene(bool)));
    connect(_ui->actionWindowProject, SIGNAL(triggered(bool)), this, SLOT(onWindowProject(bool)));
    connect(_ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(onFullscreen()));

    // Initialize the selection begin and end to offscreen which means no selection.
    _selectionBegin = new gameplay::Vector2(-1, -1);
    _selectionEnd = new gameplay::Vector2(-1, -1);

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

void EditorWindow::onNewProject()
{
    _projectWizard->show();
    _projectWizard->onNewProjectPressed();
}

void EditorWindow::onOpenProject()
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
        setWindowTitle(QString(QLatin1String(TYPE_WINDOW_TITLE)) +
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

void EditorWindow::onTransformPan(bool checked)
{
}

void EditorWindow::onTransformMove(bool checked)
{
}

void EditorWindow::onTransformRotate(bool checked)
{
}

void EditorWindow::onTransformScale(bool checked)
{
}


void EditorWindow::onTransformModeWorld()
{

    _transformModeButton->setDefaultAction(_ui->actionTransformModeWorld);

    // TODO:
}

void EditorWindow::onTransformModeLocal()
{
    _transformModeButton->setDefaultAction(_ui->actionTransformModeLocal);
    // TODO:
}


void EditorWindow::onShadingLit()
{
    _shadingButton->setDefaultAction(_ui->actionShadingLit);
    // TODO:
}

void EditorWindow::onShadingUnlit()
{

    _shadingButton->setDefaultAction(_ui->actionShadingUnlit);
    // TODO:
}

void EditorWindow::onShadingWireframe()
{
    _shadingButton->setDefaultAction(_ui->actionShadingWireframe);
    // TODO:
}

void EditorWindow::onShowGrid(bool show)
{
    // TODO
}

void EditorWindow::onShowBounds(bool show)
{
    // TODO:
}

void EditorWindow::onWindowProperties(bool visible)
{
    _ui->dockWidgetProperties->show();
}

void EditorWindow::onWindowScene(bool visible)
{
    _ui->dockWidgetScene->show();
}

void EditorWindow::onWindowProject(bool visible)
{
    _ui->dockWidgetProject->show();
}

void EditorWindow::onFullscreen()
{
    setFullscreen(true);
}

bool EditorWindow::isFullscreen() const
{
    return _fullscreen;
}

void EditorWindow::setFullscreen(bool fullscreen)
{
    if (fullscreen && !_fullscreen)
    {
        _savedGeometry = saveGeometry();
        _savedState = saveState();
        _ui->gameView->setParent(nullptr);
        _ui->gameView->showFullScreen();
        _ui->gameView->setFocus();
        _fullscreen = true;
    }
    else
    {
        _ui->tabWidget->addTab(_ui->gameView, "Viewport");
        _ui->gameView->showNormal();
        restoreGeometry(_savedGeometry);
        restoreState(_savedState);
        _fullscreen = false;
    }
}

