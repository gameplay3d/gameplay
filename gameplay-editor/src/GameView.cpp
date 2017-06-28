#include "GameView.h"
#include <QtWidgets>

GameView::GameView(QWidget* parent) : QWidget(parent),
    _editor(nullptr),
    _graphics(nullptr),
    _scene(nullptr),
    _wireframe(false)
{
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);

    _graphics = gameplay::Graphics::getGraphics();
    if (!_graphics->isInitialized())
        _graphics->initialize((unsigned long)winId());

    _graphics->resize(geometry().width(), geometry().height());

    this->connect(&_timer, SIGNAL(timeout()), SLOT(onTimer()));

    _timer.start(0);
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
}

void GameView::onFinalize()
{
}

void GameView::onUpdate(float elapsedTime)
{
}

void GameView::onRender(float elapsedTime)
{
}

void GameView::onTimer()
{
    if ((_size != geometry().size()) && (QApplication::mouseButtons() == Qt::NoButton))
    {
        _size = geometry().size();
        _graphics->resize(geometry().width(), geometry().height());
        setGeometry(geometry().x(), geometry().y(), geometry().width(), geometry().height());
    }
    gameplay::Game::onFrame();
}

void GameView::paintEvent(QPaintEvent* evt)
{
    QPainter p(this);
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);
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



