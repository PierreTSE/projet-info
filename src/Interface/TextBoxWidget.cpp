#include "TextBoxWidget.hpp"
#include <functional>

TextBoxWidget::TextBoxWidget(size_t fontSize) :
    fontSize_{fontSize}
{}

Widget::img TextBoxWidget::actualRender() const
{
	return img();
}

void TextBoxWidget::actualResize(const dim_t & size)
{
}

bool TextBoxWidget::actualPropagateEvent(const Event& event)
{
	return false;
}


