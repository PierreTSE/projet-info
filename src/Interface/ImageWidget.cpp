#include "ImageWidget.hpp"


Widget::img ImageWidget::actualRender() const
{
    img render;
    if(image_.isSelected() && is_selectable_)
    {
        // This code very nice : couleur hardcodée
        render.assign(size_.x, size_.y, 1, 3, 38);
        render.draw_image(0, 0, 0, 1, img(size_.x, size_.y, 1, 1, 141));
        render.draw_image(0, 0, 0, 2, img(size_.x, size_.y, 1, 1, 175));
    }
    else
    {
        render.assign(size_.x, size_.y, 1, 3, 255);
    }

    const double x_ratio = static_cast<double>(size_.x - 10) / image_.getImgPtr()->width();
    const double y_ratio = static_cast<double>(size_.y - 10) / image_.getImgPtr()->height();
    
    if(x_ratio < y_ratio)
    {
        img temp = *image_.getImgPtr();
        temp.resize(size_.x-10, image_.getImgPtr()->height()*x_ratio, 1, 3, 2);
        render.draw_image((size_.x-temp.width())/2, (size_.y-temp.height())/2, 0, 0, temp);
    }
    else
    {
        img temp = *image_.getImgPtr();
        temp.resize(image_.getImgPtr()->width()*y_ratio, size_.y-10, 1, 3, 2);
        render.draw_image((size_.x-temp.width())/2, (size_.y-temp.height())/2, 0, 0, temp);
    }
    
    return render;
}

void ImageWidget::actualResize(const dim_t& size)
{
    size_ = size;
    callRedraw();
}

dim_t ImageWidget::actualSize() const
{
    return size_;
}

bool ImageWidget::actualPropagateEvent(const Event& event)
{    
    if (std::holds_alternative<ClickEvent>(event.event))
    {
        if(std::get<ClickEvent>(event.event).type == ClickEvent::LEFT && is_selectable_)
            image_.select(!image_.isSelected());
        callRedraw();
        return callback_(std::get<ClickEvent>(event.event), this);
    }
    // TODO Gérer les events
    return false;
}
