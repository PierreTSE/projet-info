#include "ButtonWidget.hpp"
#include <iostream>

ButtonWidget::img ButtonWidget::actualRender() const
{
	return img;
}

void ButtonWidget::actualResize(const dim_t & size)
{
	size_ = size;
	content_->resize(size);
	needRedraw();
}

bool ButtonWidget::actualPropagateEvent(const Event& event)
{
	if (content_->isInside(event.first))
		if (content_->propagateEvent(event))
			return true;

	if (std::holds_alternative<ClickEvent>(event.second))
	{
        if(std::get<ClickEvent>(event.second).type == ClickEvent::LEFT)
        {
			std::cerr << "thomas est left méchant";
        }
		if (std::get<ClickEvent>(event.second).type == ClickEvent::RIGHT)
		{
			std::cerr << "thomas est right méchant";
		}
		return true;
	}
	return false;
}
