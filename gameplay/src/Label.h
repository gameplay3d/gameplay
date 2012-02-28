#ifndef LABEL_H_
#define LABEL_H_

#include "Control.h"
#include "Theme.h"

namespace gameplay
{

class Label : public Control
{
public:
    static Label* create(Theme::Style* style, Properties* properties);
    static Label* getLabel(const char* id);
    
    void setText(const char* text);
    const char* getText();

    void update(const Vector2& position);

    void drawText(const Vector2& position);

protected:

    Label();
    Label(const Label& copy);
    virtual ~Label();

    virtual void init(Theme::Style* style, Properties* properties);

    std::string _text;
    Rectangle _viewport;
};

}

#endif