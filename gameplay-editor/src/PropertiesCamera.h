#pragma once

#include "gameplay.h"
#include <QWidget>

namespace Ui
{
class PropertiesCamera;
}

/**
 * Defines widgets for editing camera properties.
 */
class PropertiesCamera : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesCamera(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~PropertiesCamera();

    /**
     * Sets the camera to edit properties for.
     *
     * @param camera The camera to edit the properties for.
     */
    void setCamera(std::shared_ptr<gameplay::Camera> camera);

    /**
     * Gets the header name for the properties component editor.
     *
     * @return The header name for the properties component editor.
     */
    QString getHeaderName() const;

    /**
     * Gets the header icon (path) for the properties component editor.
     *
     * @return The header icon (path) for the properties component editor.
     */
    QString getHeaderIcon() const;

public slots:

    void onReset(bool);

    void onModeChanged(int index);

    void onColorPressed();

    void onFieldOfViewSliderChanged(int value);

    void onFieldOfViewValueChanged(int value);

private:

    void updateProperties();
    void setColor(QColor color);

    Ui::PropertiesCamera* _ui;
    std::shared_ptr<gameplay::Camera> _camera;
};

