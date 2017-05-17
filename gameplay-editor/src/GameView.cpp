#include "GameView.h"
#include <QtWidgets>

GameView::GameView(QWidget* parent) : QWidget(parent),
    _editor(nullptr),
    _scene(nullptr),
    _wireframe(false)
{
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
}

GameView::~GameView()
{
    Game::getInstance()->exit();
}

void GameView::setEditor(EditorWindow* editor)
{
    _editor = editor;
}

void GameView::onSceneChanged()
{
    _scene = _editor->getScene();
}

void GameView::onInitialize()
{
    Graphics* graphics = Graphics::getGraphics();
    if (!graphics->isInitialized())
        graphics->initialize((unsigned long)winId());

    Game::onInitialize();
}

void GameView::onFinalize()
{
    Game::onFinalize();
}

void GameView::onUpdate(float elapsedTime)
{
    Game::onUpdate(elapsedTime);
}

void GameView::onRender(float elapsedTime)
{
    Game::onRender(elapsedTime);
}

void GameView::paintEvent(QPaintEvent* evt) 
{
    Game::onFrame();
}

void GameView::mousePressEvent(QMouseEvent* evt)
{
    // TODO: Handler here...
}

void GameView::mouseReleaseEvent(QMouseEvent* evt)
{
    // TODO: Handler here...
}

void GameView::keyPressEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_Escape && !evt->isAutoRepeat())
    {
        if (_editor->isFullscreen())
        {
            _editor->setFullscreen(false);
        }
    }
}

void GameView::keyReleaseEvent(QKeyEvent* evt)
{
    // TODO: Handler here...
}


