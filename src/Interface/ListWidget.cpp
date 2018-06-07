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
		buttons_.emplace_back(str, false, fontSize_);
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
			std::clog << "ListWidget column construction : adapted width to length : " << size_.x << std::endl;
		}
		else
		{
		    //TODO cas où la ligne est demandée plus large que par défaut
		}

		if (size.y <= fontSize_)
		{
			size_.y = fontSize_;
			buttonCoordinates_.y.push_back(size_.y);
			std::clog << "ListWidget column construction : adapted height to length : " << size_.y << std::endl;
		}
		else
		{
		    //TODO cas où la ligne est demandée plus haute que par défaut
			std::clog << "La ligne est demandée plus haute que par defaut" << std::endl;
		}
	}
	else //if(column_)
    {
		bool is_default_size = false;

        const long default_size_x = std::max_element(buttons_.begin(), buttons_.end(), [](ButtonWidget b1, ButtonWidget b2) {return b1.size().x < b2.size().x; })->size().x;
		if (size.x <= default_size_x)
		{
			size_.x = default_size_x;
			buttonCoordinates_.x.push_back(size_.x);
			is_default_size = true;
			std::clog << "ListWidget column construction : adapted width to length : " << size_.x << std::endl;
		}
		else
		{
		    //TODO cas où la colonne est demandée plus large que par défaut
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
			is_default_size = true;
		    std::clog << "ListWidget column construction : adapted height to length : " << size_.y << std::endl;
		}
		else
		{
		    //TODO cas où la colonne est demandée plus haute que par défaut
		}

		for (auto& button : buttons_)
		{
			button.resize({ size_.x, fontSize_ });
		}
    }
}

ListWidget::img ListWidget::actualRender() const
{
	img render(size_.x, size_.y, 1, 3, 255);
    if(column_)
    {
		int posY = 0;
		for (size_t i = 0; i < buttons_.size(); i++)
		{
            if(buttons_[i].isColored())
				render.draw_rectangle(0, posY, 0, size_.x, posY + buttons_[i].size().y - 1, 0, buttons_[i].getBackgroundColor());
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

ListWidget::ListWidget(const ListWidget& other) :
	size_{other.size_},
	buttons_{other.buttons_},
	column_{other.column_},
	fontSize_{other.fontSize_},
	is_hovered_{other.is_hovered_},
	buttonCoordinates_{other.buttonCoordinates_}
{
	for(auto& button : buttons_)
		button.setParent(this);
}
