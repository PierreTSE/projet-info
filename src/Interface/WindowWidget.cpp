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

void WindowWidget::manageEvents()
{
    dim_t mouse{window_.mouse_x(), window_.mouse_y()};
    if(mouse != lastMouse_)
        propagateEvent(Event{mouse, MoveEvent{lastMouse_}});
    if(window_.button()&1 != lastButton_&1)
        propagateEvent();

}
