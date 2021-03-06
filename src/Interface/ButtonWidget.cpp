#include "ButtonWidget.hpp"
#include "../Utilities/Utilities.hpp"
#include <iostream>

ButtonWidget::ButtonWidget(const std::string& text, bool clickable, const int& fontSize, const dim_t& size) :
    size_{ size },
    holds_click_{clickable},
    text_{text},
    fontSize_{fontSize}
{
	img imgtext;
	const unsigned char color = 1;
	imgtext.draw_text(0, 0, static_cast<const char* const>(UTF8toISO8859_1(text).c_str()), &color, 0, 1, fontSize_);

	if (size.y < imgtext.height())
	{
		size_.y = imgtext.height();
		std::clog << "ButtonWidget construction : adapted height to font size : "<< fontSize_ << std::endl;
	}
	if (size.x < imgtext.width())
	{
		size_.x = imgtext.width();
		std::clog << "ButtonWidget construction : adapted width to text length : " << text << std::endl;
	}
}

ButtonWidget::img ButtonWidget::actualRender() const
{
	const unsigned char white[] = { 255, 255, 255 }, black[] = { 0, 0, 0 };
	img render(size_.x, size_.y, 1, 3, 255);
    
    if(is_hovered_ || is_clicked_)
    {
		render.draw_rectangle(0, 0, size_.x, size_.y, backgroundColor_);
        render.draw_text(0, 0, static_cast<const char* const>(UTF8toISO8859_1(text_).c_str()), white, backgroundColor_, 100, fontSize_);
		//render.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), white, backblue, 100, size_.y);
    }
	else
	{
		render.draw_text(0, 0, static_cast<const char* const>(UTF8toISO8859_1(text_).c_str()), black, white, 100, fontSize_);
		//render.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), black, white, 100, size_.y);
	}

	render.resize(size_.x, size_.y, -100, -100, 3);

	return render;
}

void ButtonWidget::actualResize(const dim_t & size)
{
	size_ = size;
	callRedraw();
}

bool ButtonWidget::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<ClickEvent>(event.event) && std::get<ClickEvent>(event.event).type == ClickEvent::LEFT)
	{
        if(holds_click_)
        {
			if (isInside(event.pos))
			{
				is_clicked_ = !is_clicked_;
				callRedraw();
				return callBack(std::get<ClickEvent>(event.event), this);
			}
			else
			{
				is_clicked_ = false;
				return false;
			}
        }
		else if (isInside(event.pos))
		{
			return callBack(std::get<ClickEvent>(event.event), this);
		}
	}

	if (std::holds_alternative<MoveEvent>(event.event))
	{
		if (isInside(event.pos))
		{
			is_hovered_ = true;
			callRedraw();
		}
		else if(is_hovered_)
        {
			is_hovered_ = false;
			callRedraw();
        }

		return false;
	}

	return false;
}