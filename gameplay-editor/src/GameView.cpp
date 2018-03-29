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

    // Initiaize

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
    //_graphics->onUpdate(elapsedTime);
}

void GameView::onTimer()
{
    if ((_size != geometry().size()) && (QApplication::mouseButtons() == Qt::NoButton))
    {
        _size = geometry().size();
        //_graphics->onResize(geometry().width(), geometry().height());
        setGeometry(geometry().x(), geometry().y(), geometry().width(), geometry().height());
    }
}

void GameView::paintEvent(QPaintEvent* evt)
{
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);
}

void GameView::resizeEvent(QResizeEvent* evt)
{
   //_graphics->onResize(geometry().width(), geometry().height());

   // Make sure mouse button state is 'up' after resize.
   Input::getInput()->postMousePressEvent(_mousePosition.x(),
                                          _mousePosition.y(),
                                          _mouseScroll,
                                          Input::MouseButton::eLeft,
                                          false);
   Input::getInput()->postMousePressEvent(_mousePosition.x(),
                                          _mousePosition.y(),
                                          _mouseScroll, Input::MouseButton::eRight, false);
 }

void GameView::mousePressEvent(QMouseEvent* evt)
{
    const bool down = true;
    _mousePosition = QPoint(evt->x(), evt->y());
    Input::getInput()->postMousePressEvent((int32_t) evt->x(),
                                           (int32_t) evt->y(),
                                           (int32_t) _mouseScroll,
                                           translateMouseButton(evt->buttons()),
                                           down);
    }

void GameView::mouseReleaseEvent(QMouseEvent* evt)
{
    const bool down = false;
    _mousePosition = QPoint(evt->x(), evt->y());
    Input::getInput()->postMousePressEvent((int32_t) evt->x(),
                                           (int32_t) evt->y(),
                                           (int32_t) _mouseScroll,
                                           translateMouseButton(evt->button()),
                                           down);
}

void GameView::mouseMoveEvent(QMouseEvent* evt)
{
      _mousePosition = QPoint(evt->x(), evt->y());
      Input::getInput()->postMouseMotionEvent((int32_t) evt->x(),
                                              (int32_t) evt->y(),
                                               _mouseScroll);
}

void GameView::mouseWheelEvent(QWheelEvent* evt)
{
    const float step = evt->delta() / 240.0;
    _mousePosition = QPoint(evt->x(), evt->y());
    _mouseScroll += step;
    Input::getInput()->postMouseMotionEvent((int32_t) evt->x(),
                                           (int32_t) evt->y(),
                                           (int32_t) _mouseScroll);
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

    // TODO: Handler here...
}

void GameView::keyReleaseEvent(QKeyEvent* evt)
{
    // TODO: Handler here...
}

void GameView::closeEvent(QCloseEvent* evt)
{
}

Input::MouseButton GameView::translateMouseButton(Qt::MouseButtons buttons)
{
    if (buttons & Qt::LeftButton)
    {
         return Input::MouseButton::eLeft;
    }
    else if (buttons & Qt::RightButton)
    {
        return Input::MouseButton::eRight;
    }
    return Input::MouseButton::eMiddle;
}

Input::MouseButton GameView::translateMouseButton(Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
    {
        return Input::MouseButton::eLeft;
    }
    else if (button == Qt::RightButton)
    {
        return Input::MouseButton::eRight;
    }
    return Input::MouseButton::eMiddle;
}




