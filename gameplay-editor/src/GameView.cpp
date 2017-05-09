#include "GameView.h"
#include <QtWidgets>

GameView::GameView(QWidget* parent) : QWidget(parent),
    _editor(nullptr),
    _scene(nullptr),
    _wireframe(false)
{
}

GameView::~GameView()
{
    Game::getInstance()->exit();
}

void GameView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void GameView::assetPathChanged(const QString& path)
{
    QString fixedPath(path);
    fixedPath.append("/");
    QByteArray ba = fixedPath.toLatin1();
    const char* str = ba.data();
    FileSystem::setAssetPath(str);
}

void GameView::sceneChanged()
{
    _scene = _editor->getScene();
}


void GameView::onInitialize()
{
    // Set the asset path for the editor specific resources.
    // We will copy the this projects res folder and
    // All resource use will be Ex. "res/foo.png"
    FileSystem::setAssetPath("./");

    // Initalized the graphics system
    Graphics::getGraphics()->initialize((unsigned long)winId());

    Logger::log(Logger::LEVEL_INFO, "GameView initialized.\n");
}

void GameView::onFinalize()
{
    Logger::log(Logger::LEVEL_INFO, "GameView finalized.\n");
}

void GameView::onUpdate(float elapsedTime)
{
    //Update tehe scene
}

void GameView::onRender(float elapsedTime)
{

    //clear(

    // draw
}


void GameView::mousePressEvent(QMouseEvent* event)
{
    // TODO: Handler here...
}

void GameView::mouseReleaseEvent(QMouseEvent* event)
{
    // TODO: Handler here...
}

void GameView::keyPressEvent(QKeyEvent* event)
{
    // TODO: Handler here...
}

void GameView::keyReleaseEvent(QKeyEvent* event)
{
    // TODO: Handler here...
}


