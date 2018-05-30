#include <cmath>
#include "ListWidget.hpp"


void ListWidget::actualResize(const dim_t& size)
{
    if(size.x != targetWidth_ || size.y != minHeight_)
    {
        targetWidth_ = size.x;
        minHeight_ = size.y;
        needRedraw();
    }
}

Widget::img ListWidget::actualRender() const
{ // TODO Mettre la méthode const en utilisant les const_iterator
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    img rendered(targetWidth_, height);
    auto it = collec_.begin();
    for(int i = 0; i < nb_row && it != collec_.end(); ++i)
    {
        for(int j = 0; j < elem_per_row && it != collec_.end(); ++j)
        {
            rendered.draw_image(j*elemSize_.x, i*elemSize_.y, 0, 0, *(it->getImgPtr()));
            ++it;
        }
    }
        
    return rendered;
}
