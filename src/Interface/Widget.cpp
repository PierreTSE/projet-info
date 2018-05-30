#include "Widget.hpp"

const Widget::img& Widget::render() // TODO Remettre les const quand les const_iterator sont là
{
	if(needRedraw_)
	{
		cachedImg_ = std::move(actualRender());
		needRedraw_ = false;
	}

	return cachedImg_;
}

void Widget::resize(const dim_t& size)
{
	actualResize(size);
}

dim_t Widget::size() const
{
    return actualSize();
}

bool Widget::propagateEvent(const Event& event)
{
    return actualPropagateEvent(event);
}

bool Widget::isInside(const dim_t& pos) const
{
    return actualIsInside(pos);
}


bool Widget::actualIsInside(const dim_t& pos) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < size().x && pos.y < size().y;
}
