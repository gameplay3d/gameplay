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
    gameplay::Game::getInstance()->exit();
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
    gameplay::Graphics* graphics = gameplay::Graphics::getGraphics();
    if (!graphics->isInitialized())
        graphics->initialize((unsigned long)winId());

    gameplay::Game::onInitialize();
}

void GameView::onFinalize()
{
    gameplay::Game::onFinalize();
}

void GameView::onUpdate(float elapsedTime)
{
    gameplay::Game::onUpdate(elapsedTime);
}

void GameView::onRender(float elapsedTime)
{
    gameplay::Game::onRender(elapsedTime);
}

void GameView::paintEvent(QPaintEvent* evt) 
{
    gameplay::Game::onFrame();
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

void GameView::resizeEvent(QResizeEvent* evt)
{
    QWidget::resizeEvent(evt);
}


