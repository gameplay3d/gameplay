#pragma once

#include "gameplay.h"
#include <QWidget>

namespace Ui
{
class PropertiesLight;
}

/**
 * Defines widgets for editing light properties.
 */
class PropertiesLight : public QWidget
{
    Q_OBJECT

public:

    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesLight(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~PropertiesLight();

    /**
     * Sets the light to edit properties for.
     *
     * @param light The light to edit the properties for.
     */
    void setLight(std::shared_ptr<gameplay::Light> light);

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

    void onColorPressed();

    void onTypeChanged(int index);

    void onLightingChanged(int index);

    void onShadowsChanged(int index);

    void onAngleSliderChanged(int value);

    void onAngleValueChanged(int value);

private:

    void updateProperties();
    void setColor(QColor color);

    Ui::PropertiesLight* _ui;
    std::shared_ptr<gameplay::Light> _light;
};

