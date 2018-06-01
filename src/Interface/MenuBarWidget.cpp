#include "MenuBarWidget.hpp"

MenuBarWidget::img MenuBarWidget::actualRender() const
{
	return img();
}

void MenuBarWidget::actualResize(const dim_t& size)
{
    
}

bool MenuBarWidget::actualPropagateEvent(const Event& event)
{
	//if (content_->isInside(event.first - dim_t {-delta, 0}))
	//	if (content_->propagateEvent(event))
	//		return true;
	return true;
}