#include <cmath>
#include "ListWidget.hpp"
#include <iostream> //TODO à enlever

void ListWidget::actualResize(const dim_t& size)
{
    if(size.x != targetWidth_ || size.y != minHeight_)
    {
        targetWidth_ = size.x;
        minHeight_ = size.y;
        needRedraw();
    }
}

Widget::img ListWidget::actualRender()  // TODO Remettre les const quand les const_iterator sont là
{ // TODO Mettre la méthode const en utilisant les const_iterator
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    img rendered(targetWidth_, height, 1, 3, 255);
    auto it = collec_.begin();
    
    std::cerr << nb_row << '*' << elem_per_row << std::endl;
    
    for(int i = 0; i < nb_row && it != collec_.end(); ++i)
    {
        for(int j = 0; j < elem_per_row && it != collec_.end(); ++j)
        {
            auto temp = img(*(it->getImgPtr()));
            temp.resize(elemSize_.x, elemSize_.y, 1, 3, 5);
            rendered.draw_image(j*elemSize_.x, i*elemSize_.y, 0, 0, temp);
            ++it;
        }
    }
        
    return rendered;
}

dim_t ListWidget::actualSize() const
{
    auto collec_size = std::distance(collec_.begin(), collec_.end());
    long long elem_per_row = targetWidth_ / elemSize_.x;
    long long nb_row = static_cast<long long>(ceil(collec_size/static_cast<double>(elem_per_row)));
    long long height = std::max(minHeight_, nb_row * elemSize_.y);
    return dim_t{targetWidth_, height};
}

bool ListWidget::actualPropagateEvent(const Event& event)
{
    if(std::holds_alternative<ZoomEvent>(event.second)) {
        std::cerr << "scroll :" << std::get<ZoomEvent>(event.second).amount << std::endl;
        // TODO Faire des tests
        int amount = std::get<ZoomEvent>(event.second).amount * 10;
        elemSize_ = elemSize_ + dim_t{amount, amount};
        needRedraw();
        return true;
    }
    return false;
}
