#include "LayoutWidget.hpp"


LayoutWidget::LayoutWidget(Widget* contentLeft, Widget* contentRight, float ratio, dim_t size) :
    contentLeft_{contentLeft},
    contentRight_{contentRight},
    ratio_{ratio},
    size_{size}
{
    contentLeft_->resize({size_.x*ratio, size_.y});
    contentRight_->resize({size_.x*(1-ratio), size_.y});
}

Widget::img LayoutWidget::actualRender() const
{
    img render(size_.x, size_.y, 0, 3, 255);
    render.draw_image(0, 0, 0, 0, contentLeft_->render());
    render.draw_image(size_.x*ratio_, 0, 0, 0, contentRight_->render());
    
    return render;
}

void LayoutWidget::actualResize(const dim_t& size)
{
    size_ = size;
    contentLeft_->resize({size_.x*ratio_, size_.y});
    contentRight_->resize({size_.x*(1-ratio_), size_.y});
    callRedraw();
}

bool LayoutWidget::actualPropagateEvent(const Event& event)
{
    if(contentLeft_->isInside(event.pos))
        if(contentLeft_->propagateEvent(event))
            return true;
    if(contentRight_->isInside({event.pos.x-size_.x*ratio_, event.pos.y}))
        if(contentRight_->propagateEvent(Event{{event.pos.x-size_.x*ratio_, event.pos.y}, event.event}))
            return true;
    
    return false;
}
