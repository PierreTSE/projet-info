#include "ButtonWidget.hpp"
#include <iostream>

ButtonWidget::ButtonWidget(const std::string& text, const int& fontSize, const dim_t& size) :
    size_{ size },
    text_{text},
    fontSize_{fontSize}
{
	img imgtext;
	const unsigned char color = 1;
	imgtext.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), &color, 0, 1, fontSize_);

	if (size.y < imgtext.height())
	{
		size_.y = imgtext.height();
		std::cerr << "ButtonWidget construction : adapted height to font size : "<< fontSize_ << std::endl;
	}
	if (size.x < imgtext.width())
	{
		size_.x = imgtext.width();
		std::cerr << "ButtonWidget construction : adapted width to text length : " << text << std::endl;
	}
}

ButtonWidget::img ButtonWidget::actualRender() const
{
	const unsigned char white[] = { 255, 255, 255 }, grey[] = { 128,128,128 }, backblue[] = { 102, 153, 255 }, black[] = { 0, 0, 0 };
	img render(size_.x, size_.y, 1, 3);
    
    if(is_hovered_ || is_clicked_)
    {
        render.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), white, backblue, 100, fontSize_);
		//render.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), white, backblue, 100, size_.y);
    }
	else
	{
		render.draw_text(0, 0, static_cast<const char* const>(text_.c_str()), black, white, 100, fontSize_);
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
	if (std::holds_alternative<ClickEvent>(event.event))
	{
		if (isInside(event.pos))
		{

			if (std::get<ClickEvent>(event.event).type == ClickEvent::LEFT)
			{
				std::cerr << "thomas est left méchant" << std::endl; //HACK test
				is_clicked_ = !is_clicked_;
				callRedraw();
				if (!callBack(std::get<ClickEvent>(event.event),this)) throw std::runtime_error("Button could not left click.");
			}
			else if (std::get<ClickEvent>(event.event).type == ClickEvent::MIDDLE)
			{
				std::cerr << "thomas est middle méchant" << std::endl; //HACK test
				is_clicked_ = !is_clicked_;
				callRedraw();
				if (!callBack(std::get<ClickEvent>(event.event), this)) throw std::runtime_error("Button could not middle click.");
			}
			else if (std::get<ClickEvent>(event.event).type == ClickEvent::RIGHT)
			{
				std::cerr << "thomas est right méchant" << std::endl; //HACK test
				is_clicked_ = !is_clicked_;
				callRedraw();
				if (!callBack(std::get<ClickEvent>(event.event), this)) throw std::runtime_error("Button could not right click.");
			}
			else
			{
				is_clicked_ = false;
			}
			return true;
		}
		else
		{
			is_clicked_ = false;
			return false;
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