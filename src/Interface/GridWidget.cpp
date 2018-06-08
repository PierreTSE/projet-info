#include <cmath>
#include "GridWidget.hpp"


void GridWidget::actualResize(const dim_t& size)
{
    if(size.x != targetWidth_ || size.y != minHeight_)
    {
        targetWidth_ = size.x;
        minHeight_ = size.y;
        callRedraw();
    }
}

Widget::img GridWidget::actualRender() const
{ 
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    img rendered(targetWidth_, height, 1, 3, 255);
    auto it = collec_.begin();
    
    for(int i = 0; i < nb_row && it != collec_.end(); ++i)
    {
        for(int j = 0; j < elem_per_row && it != collec_.end(); ++j)
        {
            if(associatedWidgets_.find(it->getPath()) == associatedWidgets_.end())
            {
                ImageWidget temp(*it, elemSize_, callback_);
                temp.setParent(const_cast<Widget*>(static_cast<const Widget*>(this))); // HACK
                associatedWidgets_.insert({it->getPath(), std::move(temp)});
            }
            rendered.draw_image(j*elemSize_.x, i*elemSize_.y, 0, 0, associatedWidgets_.at(it->getPath()).render());
            ++it;
        }
    }
        
    return rendered;
}

dim_t GridWidget::actualSize() const
{
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    return dim_t{targetWidth_, height};
}

bool GridWidget::actualPropagateEvent(const Event& event)
{
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    auto it = collec_.begin();

    for(int i = 0; i < nb_row && it != collec_.end(); ++i)
    {
        for(int j = 0; j < elem_per_row && it != collec_.end(); ++j)
        {
            if(associatedWidgets_.find(it->getPath()) == associatedWidgets_.end())
            {
                ImageWidget temp(*it, elemSize_, callback_);
                temp.setParent(this);
                associatedWidgets_.insert({it->getPath(), std::move(temp)});
            }
            if(associatedWidgets_.at(it->getPath()).isInside(event.pos - dim_t{j*elemSize_.x, i*elemSize_.y}))
            {
                auto event_temp = event;
                event_temp.pos = event_temp.pos - dim_t{j*elemSize_.x, i*elemSize_.y};
                if(associatedWidgets_.at(it->getPath()).propagateEvent(event_temp))
                    return true;
            }
            ++it;
        }
    }
        
    if(std::holds_alternative<ZoomEvent>(event.event)) {
        // TODO Faire des tests
        const int amount = std::get<ZoomEvent>(event.event).amount * 50;
        const dim_t elemSize = elemSize_ + dim_t{ amount, amount };
		if (elemSize.x > 50 && elemSize.y > 50 && elemSize.x < 500 && elemSize.y < 500)
		{
			elemSize_ = elemSize;
			callRedraw();
			for (auto& p : associatedWidgets_)
				p.second.resize(elemSize_);
		}
        return true;
    }
    return false;
}
