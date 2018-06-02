#include "WindowWidget.hpp"


WindowWidget::WindowWidget(Widget& content, dim_t size) :
    content_{&content},
    size_{size},
    window_{size.x, size.y}
{
    content.setParent(this);
    content.resize(size);
    lastWheel_ = window_.wheel();
    lastButton_ = window_.button();
    lastMouse_ = dim_t{window_.mouse_x(), window_.mouse_y()};
}

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
    if(window_.button()&1 != lastButton_&1 && window_.button()&1)
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::LEFT}});
    if(window_.button()&2 != lastButton_&2 && window_.button()&2)
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::RIGHT}});
    if(window_.button()&4 != lastButton_&4 && window_.button()&4)
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::MIDDLE}});
    if(window_.wheel() != lastWheel_)
    {
        if(window_.is_keyCTRLLEFT() || window_.is_keyCTRLRIGHT())
            propagateEvent(Event{mouse, ZoomEvent{window_.wheel() - lastWheel_}});
        else
            propagateEvent(Event{mouse, ScrollEvent{window_.wheel() - lastWheel_}});
    }
    
    lastMouse_ = mouse;
    lastButton_ = window_.button();
    lastWheel_ = window_.wheel();
    
    if(window_.is_resized())
    {
        window_.resize();
        resize({window_.window_width(), window_.window_height()});
    }
}
