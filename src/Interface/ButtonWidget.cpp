#include "ButtonWidget.hpp"
#include <iostream>

ButtonWidget::ButtonWidget(Widget& content, const dim_t& size, const std::string& text) : 
    content_{ &content },
    size_{ size },
    text_{text}
{
	if (size.x < 13 * fontSize_)
		size_.x = 13 * fontSize_ + 1;
	content_->resize(size_);
    content_->setParent(this);
}

ButtonWidget::img ButtonWidget::actualRender() const
{
	const unsigned char white[] = { 255, 255, 255 }, grey[] = { 128,128,128 }, backblue[] = { 102, 153, 255 }, black[] = { 0, 0, 0 };
    img render(size_.x, size_.y, 1, 3, 0);
    
	const unsigned char testColor[] = { 138, 0, 230 };

    if(is_hovered && !is_clicked)
    {
        //TODO couleur de police		
        //render.draw_text(1, size_.y / 2 - fontSize_, static_cast<const char* const>(text_.c_str()), testColor, backblue, 100, fontSize_);
        //HACK pas de hover
		render.draw_text(1, size_.y / 2 - fontSize_, static_cast<const char* const>(text_.c_str()), black, white, 100, fontSize_);
    }
	else
	{
		render.draw_text(1, size_.y / 2 - fontSize_, static_cast<const char* const>(text_.c_str()), black, white, 100, fontSize_);
	}

	return render;
}

void ButtonWidget::actualResize(const dim_t & size)
{
	size_ = size;
	content_->resize(size);
	callRedraw();
}

bool ButtonWidget::actualPropagateEvent(const Event& event)
{
	if (content_->isInside(event.pos))
		if (content_->propagateEvent(event))
			return true;

	if (std::holds_alternative<ClickEvent>(event.event))
	{
		if (isInside(event.pos))
		{

			if (std::get<ClickEvent>(event.event).type == ClickEvent::LEFT)
			{
				std::cerr << "thomas est left méchant" << std::endl;
				is_clicked = true;
				callRedraw();
				content_->propagateEvent(event);
				return true;
			}
			else if (std::get<ClickEvent>(event.event).type == ClickEvent::RIGHT)
			{
				std::cerr << "thomas est right méchant" << std::endl;
			}
			else
			{
				std::cerr << "thomas est middle méchant" << std::endl;
			}
			return true;
		}
		else
		{
			is_clicked = false;
			return false;
		}
	}
	if (std::holds_alternative<MoveEvent>(event.event))
	{
		if (isInside(event.pos))
		{
			is_hovered = true;
			callRedraw();
		}
		else
			is_hovered = false;
	}

	return false;
}
