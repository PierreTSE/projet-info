#include "ScrollWidget.hpp"
#include <algorithm>
#include <iostream>


void ScrollWidget::actualResize(const dim_t& size)
{
    size_ = size;
    content_->resize(size);
    if(content_->size().y > size.y)
        content_->resize(size - dim_t{scrollBarWidth, 0});
    callRedraw();
}

dim_t ScrollWidget::actualSize() const
{
    return size_;
}

Widget::img ScrollWidget::actualRender() const
{
    img render(size_.x, size_.y, 1, 3, 255);
    if(size_.y >= content_->size().y)
    {
        // Pas de scrollbar
        render.draw_image(0, 0, 0, 0, content_->render());
    }
    else
    {
        // Avec une scrollbar
        auto temp = content_->render().get_crop(0, delta, 0, 0, size_.x-scrollBarWidth-1, delta+size_.y, 0, 2);
        render.draw_image(0, 0, 0, 0, temp);
        long long barHeight = static_cast<double>(size_.y) / content_->size().y * size_.y;
        long long pos = static_cast<double>(size_.y) / content_->size().y * delta;
        render.draw_image(size_.x-scrollBarWidth, pos, 0, 0, img(scrollBarWidth, barHeight, 1, 3, 200));
        std::cerr << delta << std::endl;
    }
    
    return render;
}

bool ScrollWidget::actualPropagateEvent(const Event& event)
{
    if(content_->isInside(event.pos - dim_t{0, -delta}))
        if(content_->propagateEvent(event))
            return true;
    if(std::holds_alternative<ScrollEvent>(event.event)) {
        delta -= std::get<ScrollEvent>(event.event).amount * 15;
        delta = std::clamp<int>(delta, 0, content_->size().y-size_.y);
        callRedraw();
        return true;
    }
    return false;
}
