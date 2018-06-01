#include "ButtonWidget.hpp"
#include <iostream>

ButtonWidget::img ButtonWidget::actualRender() const
{
	img render(size_.x, size_.y, 1, 3, 200);
	size_t fontHeight = 13;
	const unsigned char white[] = { 255, 255,255 }, grey[] = { 128,128,128 };
	render.draw_text(1, size_.y / 2 - fontHeight, static_cast<const char* const>("mon bouton"), white, grey, 100, fontHeight);

	return render;
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
			std::cerr << "thomas est left méchant" << std::endl;
        }
		if (std::get<ClickEvent>(event.second).type == ClickEvent::RIGHT)
		{
			std::cerr << "thomas est right méchant" << std::endl;
		}
		return true;
	}
	return false;
}
