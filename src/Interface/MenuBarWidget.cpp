#include "MenuBarWidget.hpp"


MenuBarWidget::MenuBarWidget(Widget* content, ListWidget* buttons, const dim_t& size) :
	size_{size},
	content_{content},
	buttons_{buttons}
{
    if(content_)
        content_->setParent(this);
    buttons_->setParent(this);
}

MenuBarWidget::img MenuBarWidget::actualRender() const
{
	img render(size_.x, size_.y, 1, 3, 255);
	render.draw_image(0, 0, 0, 0, buttons_->render());
	if(content_)
		render.draw_image(0, buttons_->size().y, 0, 0, content_->render());
	
	return render;
}

void MenuBarWidget::actualResize(const dim_t& size)
{
    size_ = size;
    callRedraw();
	if(content_)
    	content_->resize(size - dim_t{0, buttons_->size().y});
}

bool MenuBarWidget::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<MoveEvent>(event.event))
	{
		if (buttons_->propagateEvent(event))
			return true;
		else
			if (content_ && content_->isInside(event.pos - dim_t {0, buttons_->size().y}))
				return content_->propagateEvent(Event{ event.pos - dim_t {0, buttons_->size().y}, MoveEvent{std::get<MoveEvent>(event.event).lastPos - dim_t {0, buttons_->size().y}} });
	}
    
	if(buttons_->isInside(event.pos))
		return buttons_->propagateEvent(event);
	else if(content_ && content_->isInside(event.pos - dim_t{0, buttons_->size().y}))
		return content_->propagateEvent(Event{event.pos - dim_t{0, buttons_->size().y}, event.event});
	
	return false;
}
