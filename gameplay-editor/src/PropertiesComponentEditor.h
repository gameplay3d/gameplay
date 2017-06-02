#pragma once

#include "gameplay.h"
#include <QWidget>
#include <QLabel>

namespace Ui
{
class PropertiesComponentEditor;
}

/**
 * Defines a container for specific editor content.
 */
class PropertiesComponentEditor : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesComponentEditor(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~PropertiesComponentEditor();

    /**
     * Sets the component to be edited inside this editor.
     *
     * @param component The component to be edited inside this editor.
     */
    void setComponent(std::shared_ptr<gameplay::Component> component);

signals:

    void opened();

    void closed();

public slots:

    void onReset(bool);

    void onDetach(bool);

    void onHeaderPressed();

private:

    Ui::PropertiesComponentEditor* _ui;
    std::shared_ptr<gameplay::Component> _component;
    bool _expanded;
    int _expandedHeight;
};
