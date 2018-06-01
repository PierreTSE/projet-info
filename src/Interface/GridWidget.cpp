#include <cmath>
#include "GridWidget.hpp"


void GridWidget::actualResize(const dim_t& size)
{
    if(size.x != targetWidth_ || size.y != minHeight_)
    {
        targetWidth_ = size.x;
        minHeight_ = size.y;
        needRedraw();
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
            auto temp = img(*(it->getImgPtr()));
            temp.resize(elemSize_.x, elemSize_.y, 1, 3, 2);
            rendered.draw_image(j*elemSize_.x, i*elemSize_.y, 0, 0, temp);
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
    if(std::holds_alternative<ZoomEvent>(event.second)) {
        // TODO Faire des tests
        int amount = std::get<ZoomEvent>(event.second).amount * 10;
        elemSize_ = elemSize_ + dim_t{amount, amount};
        needRedraw();
        return true;
    }
    return false;
}
