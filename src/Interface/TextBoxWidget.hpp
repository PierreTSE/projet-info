#ifndef TEXTBOXWIDGET_HPP
#define TEXTBOXWIDGET_HPP
#include "Widget.hpp"

class TextBoxWidget : public Widget
{
    public:
		TextBoxWidget(size_t fontSize = 23);

    private:
		size_t fontSize_;
};

#endif // TEXTBOXWIDGET_HPP
