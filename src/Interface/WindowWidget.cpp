#include "WindowWidget.hpp"


Widget::img WindowWidget::actualRender() const
{
    img render = content_->render();
    return render;
}

void WindowWidget::actualResize(const dim_t& size)
{
    if(size != size_) {
        size_ = size;
        needRedraw();
        content_->resize(size);
    }
}

bool WindowWidget::actualPropagateEvent(const Event& event)
{
    return content_->propagateEvent(event);
}
