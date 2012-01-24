#ifndef LABEL_H_
#define LABEL_H_

#include "Control.h"
#include "Theme.h"

namespace gameplay
{

    class Theme;

class Label : public Control
{
public:
    static Label* create(Theme::Style* style, Properties* properties);
    static Label* getLabel(const char* id);
    
    void setText(const char* text);
    const char* getText();

    void drawText(const Vector2& position);

protected:
    Label();
    Label(const Label& copy);
    virtual ~Label();

    std::string _text;
};

}

#endif