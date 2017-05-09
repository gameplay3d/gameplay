#pragma once

#include <QFrame>

namespace Ui {
class PropertiesTransform;
}

/**
 * Properties related to object transformations.
 */
class PropertiesTransform : public QFrame
{
    Q_OBJECT
public:
    /**
     * Constructor.
     *
     * @param parent The parent widget.
     */
    explicit PropertiesTransform(QWidget* parent = 0);

    /**
     * Destructor.
     */
    ~PropertiesTransform();

    Ui::PropertiesTransform* _ui;
};
