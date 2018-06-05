#include "ListWidget.hpp"
#include <algorithm>
#include <iostream>


ListWidget::ListWidget(const std::vector<std::string>& texts, bool column, const dim_t& fontSize, const dim_t& size ) :
    size_{size},
    column_{column},
    fontSize_{fontSize}
{
    if(column_)
    {
		const std::string widestText = *std::max_element(texts.begin(), texts.end(), [](std::string str1, std::string str2) {return str1.size() < str2.size(); });
		if (size.x < fontSize.x * widestText.length())
			size_.x = fontSize_.x * widestText.length();

		if (size.y < fontSize.y * texts.size())
			size_.y = fontSize.y * texts.size();
		std::cerr << "ListWidget column construction : adapted width to text lenght:" << widestText << std::endl;

		for (const auto& str : texts)
		{
			buttons_.emplace_back(str, fontSize_.x);
			buttons_.back().setParent(this);
		}
    }
	else
	{
		if (size_.y < fontSize_.y)
			size_.y = fontSize_.y;

		size_t lineSize = 0;
		for (const auto& str : texts)
		{
			buttons_.emplace_back(str, fontSize_.x);
			lineSize += str.length() * fontSize_.x + 1;
			buttons_.back().setParent(this);
		}
		if (size_.x < lineSize)
			size_.x = lineSize;
	}

}

ListWidget::img ListWidget::actualRender() const
{
	img render(size_.x, size_.y, 1, 3);
    if(column_)
    {
		for (size_t i = 0; i < buttons_.size(); i++)
		{
			render.draw_image(0, fontSize_.y * i, 0, 0, buttons_[i].render());
		}
    }
	else
	{
		float posX = 0;
		for (size_t i = 0; i < buttons_.size(); i++)
		{

			render.draw_image(posX, 0, 1, 3, buttons_[i].render());
			posX += buttons_[i].size().x * fontSize_.x;
		}
	}
	return render;
}

void ListWidget::actualResize(const dim_t& size)
{}

bool ListWidget::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<MoveEvent>(event.event))
	{
		std::cerr << event.pos.x << "  " << event.pos.y << std::endl;
		if (column_)
		{
			for (auto& button : buttons_)
			{
				const int buttonNumber = event.pos.y / fontSize_.y;
				const Event event_offset = { { event.pos.x , event.pos.y }, event.event };
				if (button.propagateEvent(event_offset)) return true;
			}
			return false;
		}
		else throw std::runtime_error("todo"); //TODO line
	}

    if(isInside(event.pos) && std::holds_alternative<ClickEvent>(event.event))
    {
		if (column_)
		{
			const int buttonNumber = event.pos.y / fontSize_.y;
			const Event event_offset = { {event.pos.x,event.pos.y - fontSize_.y*buttonNumber},event.event };
			return buttons_[buttonNumber].propagateEvent(event_offset);
		}
		else throw std::runtime_error("todo"); //TODO line
    }

	return false;
}
