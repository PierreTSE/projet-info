#include "ButtonWidget.hpp"
#include <iostream>

ButtonWidget::ButtonWidget(const std::string& text, const dim_t& fontSize, const dim_t& size) :
    size_{ size },
    text_{text},
    fontSize_{fontSize}
{
	if (size.y < fontSize_.y)
		size_.y = fontSize_.y;
	if (size.x < text.length() * fontSize_.x)
	{
		size_.x = text.length() * fontSize_.x + 1;
		std::cerr << "ButtonWidget construction : adapted width to text lenght :" << text << std::endl;
	}
}

ButtonWidget::img ButtonWidget::actualRender() const
{
	const unsigned char white[] = { 255, 255, 255 }, grey[] = { 128,128,128 }, backblue[] = { 102, 153, 255 }, black[] = { 0, 0, 0 };
    img render(size_.x, size_.y, 1, 3, 0);
    
	const unsigned char testColor[] = { 138, 0, 230 };

    if(is_hovered && !is_clicked)
    {
        //TODO couleur de police		
        render.draw_text(0, size_.y/2 - fontSize_.y, static_cast<const char* const>(text_.c_str()), white, backblue, 100, fontSize_.x);
        //HACK pas de hover
		//render.draw_text(1, size_.y - fontSize_.y, static_cast<const char* const>(text_.c_str()), black, white, 100, fontSize_.x);
    }
	else
	{
		render.draw_text(0, size_.y/2 - fontSize_.y, static_cast<const char* const>(text_.c_str()), black, white, 100, fontSize_.x);
	}

	return render;
}

void ButtonWidget::actualResize(const dim_t & size)
{
	size_ = size;
	callRedraw();
}

bool ButtonWidget::actualPropagateEvent(const Event& event)
{
	//if (content_->isInside(event.pos))
	//	if (content_->propagateEvent(event))
	//		return true;

	if (std::holds_alternative<ClickEvent>(event.event))
	{
		if (isInside(event.pos))
		{

			if (std::get<ClickEvent>(event.event).type == ClickEvent::LEFT)
			{
				std::cerr << "thomas est left méchant" << std::endl;
				is_clicked = true;
				callRedraw();
				if (!execute(ClickEvent::LEFT)) throw std::runtime_error("Button could not left click.");
			}
			else if (std::get<ClickEvent>(event.event).type == ClickEvent::MIDDLE)
			{
				std::cerr << "thomas est middle méchant" << std::endl;
				is_clicked = true;
				callRedraw();
				if (!execute(ClickEvent::MIDDLE)) throw std::runtime_error("Button could not middle click.");
			}
			else
			{
				std::cerr << "thomas est right méchant" << std::endl;
				is_clicked = true;
				callRedraw();
				if (!execute(ClickEvent::RIGHT)) throw std::runtime_error("Button could not right click.");
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

bool ButtonWidget::execute(ClickEvent::mouseButton_t)
{
    std::cerr << "Button with no function executed." << std::endl;
	return true;
}
