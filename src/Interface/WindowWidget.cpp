#include "WindowWidget.hpp"


WindowWidget::WindowWidget(Widget* content, dim_t size) :
    content_{content},
    size_{size},
    window_{size.x, size.y}
{
    if(content) 
    {
        content_->setParent(this);
        content_->resize(size_); 
    }
    lastWheel_ = window_.wheel();
    lastButton_ = window_.button();
    lastMouse_ = dim_t{window_.mouse_x(), window_.mouse_y()};
}

Widget::img WindowWidget::actualRender() const
{
    img render = content_->render();
    if(rightClickMenu_)
        render.draw_image(rightClickMenuPos_.x, rightClickMenuPos_.y, 0, 0, rightClickMenu_->render());
    return render;
}

void WindowWidget::actualResize(const dim_t& size)
{
    if(size != size_) {
        size_ = size;
        callRedraw();
        content_->resize(size);
    }
}

bool WindowWidget::actualPropagateEvent(const Event& event)
{
    bool b = false;
    if(rightClickMenu_)
    {
        if(std::holds_alternative<MoveEvent>(event.event))
        {
            if(rightClickMenu_->propagateEvent(Event{event.pos - rightClickMenuPos_, event.event}))
                b = true;
        }
        else
        {
            if(rightClickMenu_->isInside(event.pos - rightClickMenuPos_))
                if(rightClickMenu_->propagateEvent(Event{event.pos-rightClickMenuPos_, event.event}))
                    b = true;
        }
    }
    
    if(!b)
        b = content_->propagateEvent(event);
    if(isRightMenuActive_ && rightClickMenu_ && std::holds_alternative<ClickEvent>(event.event))
    {
        rightClickMenu_.reset(nullptr);
        isRightMenuActive_ = false;
        callRedraw();
    }
    if(rightClickMenu_)
        isRightMenuActive_ = true;
    return b;
}

void WindowWidget::manageEvents()
{
    const dim_t mouse{window_.mouse_x(), window_.mouse_y()};
    if(mouse != lastMouse_)
        propagateEvent(Event{mouse, MoveEvent{lastMouse_}});
    if((window_.button()&1) != (lastButton_&1) && (window_.button()&1))
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::LEFT}});
    if((window_.button()&2) != (lastButton_&2) && (window_.button()&2))
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::RIGHT}});
    if((window_.button()&4) != (lastButton_&4) && (window_.button()&4))
        propagateEvent(Event{mouse, ClickEvent{ClickEvent::MIDDLE}});
	if ((window_.button() & 1) != (lastButton_ & 1) && !(window_.button() & 1))
		propagateEvent(Event{ mouse, UnClickEvent{ UnClickEvent::LEFT } });
	if ((window_.button() & 2) != (lastButton_ & 2) && !(window_.button() & 2))
		propagateEvent(Event{ mouse, UnClickEvent{ UnClickEvent::RIGHT } });
	if ((window_.button() & 4) != (lastButton_ & 4) && !(window_.button() & 4))
		propagateEvent(Event{ mouse, UnClickEvent{ UnClickEvent::MIDDLE } });
    if(window_.wheel() != lastWheel_)
    {
        if(window_.is_keyCTRLLEFT() || window_.is_keyCTRLRIGHT())
            propagateEvent(Event{mouse, ZoomEvent{window_.wheel() - lastWheel_}});
        else
            propagateEvent(Event{mouse, ScrollEvent{window_.wheel() - lastWheel_}});
    }
    if(window_.is_keyA() && (window_.is_keyCTRLLEFT() || window_.is_keyCTRLRIGHT()))
    {
		propagateEvent(Event{ {size_.x / 2,size_.y / 2}, SelectEvent() });
		callBack_(!(window_.is_keySHIFTLEFT() || window_.is_keySHIFTRIGHT()));
    }

    
    lastMouse_ = mouse;
    lastButton_ = window_.button();
    lastWheel_ = window_.wheel();
    
    if(window_.is_resized())
    {
		window_.resize(false);
        resize({window_.window_width(), window_.window_height()});
    }
    
    if(needRightClickMenu_)
    {
        rightClickMenuPos_ = mouse;
        needRightClickMenu_ = false;
    }
}

void WindowWidget::setContent(Widget* content)
{
    content_.reset(content);
    content_->setParent(this);
    content_->resize(size_);
    callRedraw();
}

void WindowWidget::spawnRightClickMenu(ListWidget* rightClickMenu, dim_t pos)
{
    rightClickMenu_.reset(rightClickMenu);
    rightClickMenu->setParent(this);
    if(pos == dim_t{-1, -1})
        needRightClickMenu_ = true;
    else
        rightClickMenuPos_ = pos;
}
