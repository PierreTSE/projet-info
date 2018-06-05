#include "ListWidget.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

ListWidget::ListWidget(const std::vector<std::string>& texts, const bool column, const int fontSize, const dim_t& size ) :
    size_{size},
    column_{column},
    fontSize_{fontSize}
{
	if (texts.empty())
		throw std::runtime_error("Empty ListWidget is not permitted.");

    for(const auto& str : texts)
    {
		buttons_.emplace_back(str, fontSize_);
		buttons_.back().setParent(this);
    }

	if (!column_)
	{
		auto default_size_x = 0;
        for(const auto& button : buttons_)
        {
			default_size_x += button.size().x;
			buttonCoordinates_.x.push_back(default_size_x);
        }

		if (size.x <= default_size_x)
		{
			size_.x = default_size_x;
			std::cerr << "ListWidget column construction : adapted width to length : " << size_.x << std::endl;
		}
		else
		{
		    //TODO cas o� la ligne est demand�e plus large que par d�faut
		}

		if (size.y <= fontSize_)
		{
			size_.y = fontSize_;
			buttonCoordinates_.y.push_back(size_.y);
			std::cerr << "ListWidget column construction : adapted height to length : " << size_.y << std::endl;
		}
		else
		{
		    //TODO cas o� la ligne est demand�e plus haute que par d�faut
		}
	}
	else //if(column_)
    {
        const long default_size_x = std::max_element(buttons_.begin(), buttons_.end(), [](ButtonWidget b1, ButtonWidget b2) {return b1.size().x < b2.size().x; })->size().x;
		if (size.x <= default_size_x)
		{
			size_.x = default_size_x;
			buttonCoordinates_.x.push_back(size_.x);
			std::cerr << "ListWidget column construction : adapted width to length : " << size_.x << std::endl;
		}
		else
		{
		    //TODO cas o� la colonne est demand�e plus large que par d�faut
		}

		auto default_size_y = 0;
		for (const auto& button : buttons_)
		{
			default_size_y += button.size().y;
			buttonCoordinates_.y.push_back(default_size_y);
		}

		if (size.y <= default_size_y)
		{
			size_.y = default_size_y;
		    std::cerr << "ListWidget column construction : adapted height to length : " << size_.y << std::endl;
		}
		else
		{
		    //TODO cas o� la colonne est demand�e plus haute que par d�faut
		}
    }

    /*if(column_)
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
	}*/

}

ListWidget::img ListWidget::actualRender() const
{
	img render(size_.x, size_.y, 1, 3, 255);
    if(column_)
    {
		int posY = 0;
		for (size_t i = 0; i < buttons_.size(); i++)
		{
			render.draw_image(0, posY, 0, 0, buttons_[i].render());
			posY += buttons_[i].size().y;
		}
    }
	else //if(!column_)
	{
		int posX = 0;
		for (size_t i = 0; i < buttons_.size(); i++)
		{

			render.draw_image(posX, 0, 0, 0, buttons_[i].render());
			posX += buttons_[i].size().x;
		}
	}
	return render;
}

void ListWidget::actualResize(const dim_t& size)
{
	//const float rapport_y = size.y / size_.y,
 //               rapport_x = size.x / size_.x;
 //   for(auto& x : buttonCoordinates_.x)
 //   {
	//	x *= rapport_x;
 //   }
 //   for(auto& y : buttonCoordinates_.y)
 //   {
	//	y *= rapport_y;
 //   }
}

bool ListWidget::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<MoveEvent>(event.event))
	{
		//std::cerr << event.pos.x << "  " << event.pos.y << std::endl; //HACK affiche pos
		if (column_)
		{
			if (isInside(event.pos))
			{
				for (size_t i = 0; i < buttons_.size(); ++i)
				{
					if (buttons_[i].propagateEvent(Event{ { event.pos.x, event.pos.y - buttonCoordinates_.y.at(i) }, event.event })) return true;
			    }
				is_hovered_ = true;
            }
			else if (is_hovered_ == true)
			{
			    for(auto& button: buttons_)
			    {
					button.propagateEvent(Event{ {-1,-1}, event.event });
			    }
				is_hovered_ = false;
			}
			return false;
		}
		else //if (!column_)
		{
			if (isInside(event.pos))
			{
				for (size_t i = 0; i < buttons_.size(); ++i)
				{
					if (buttons_[i].propagateEvent(Event{ { event.pos.x - buttonCoordinates_.x.at(i), event.pos.y }, event.event })) return true;
				}
				is_hovered_ = true;
			}
			else if(is_hovered_==true)
			{
				for (auto& button : buttons_)
				{
					button.propagateEvent(Event{ { -1,-1 }, MoveEvent{} });
				}
				is_hovered_ = false;
			}
			return false;
		}
	}

    if(isInside(event.pos) && std::holds_alternative<ClickEvent>(event.event))
    {
		if (column_)
		{
			const auto y_offset_it = --std::upper_bound(buttonCoordinates_.y.begin(), buttonCoordinates_.y.end(), event.pos.y);
			const auto buttonNumber = std::distance(buttonCoordinates_.y.begin(), y_offset_it);
			return buttons_[buttonNumber].propagateEvent({ { event.pos.x, event.pos.y - *y_offset_it }, event.event });
		}
		else //if(!column_)
		{
			const auto x_offset_it = --std::upper_bound(buttonCoordinates_.x.begin(), buttonCoordinates_.x.end(), event.pos.x);
			const auto buttonNumber = std::distance(buttonCoordinates_.x.begin(), x_offset_it);
			return buttons_[buttonNumber].propagateEvent({ { event.pos.x - *x_offset_it, event.pos.y }, event.event });
		}
    }
}
