#pragma once

#include "EditorWindow.h"
#include "gameplay.h"
using namespace gameplay;
#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>

/**
 * The main game view use to intialize, update and render game scenes.
 */
class GameView : public QWidget, public Game
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit GameView(QWidget* parent = nullptr);

    /**
     * Destructor.
     */
    ~GameView();

    /**
     * Sets the editor window this game view uses.
     *
     * @param editor The editor window this game view uses.
     */
    void setEditor(EditorWindow* editor);

public slots:
    /**
     * Slots handler for asset path changing.
     *
     * @param path The new asset path.
     */
    void assetPathChanged(const QString& path);

    /**
     * Handler when the scene changes.
     */
    void sceneChanged();

protected:
    /**
     * @see Game::onInitialize
     */
    void onInitialize();

    /**
     * @see Game::onFinalize
     */
    void onFinalize();

    /**
     * @see Game::onUpdate
     */
    void onUpdate(float elapsedTime);

    /**
     * @see Game::onRender
     */
    void onRender(float elapsedTime);


    /**
     * @see QWidget::paint
     */
    void paint();

    /**
     * @see QWidget::mousePressEvent
     */
    void mousePressEvent(QMouseEvent* event);

    /**
     * @see QWidget::mouseReleaseEvent
     */
    void mouseReleaseEvent(QMouseEvent* event);

    /**
     * @see QWidget::keyPressEvent
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @see QWidget::keyReleaseEvent
     */
    void keyReleaseEvent(QKeyEvent* event);


private:
    EditorWindow* _editor;
    gameplay::Graphics* _graphics;
    std::shared_ptr<gameplay::SceneObject> _scene;
    bool _wireframe;
};

