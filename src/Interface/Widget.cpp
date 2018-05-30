#include "Widget.hpp"

const Widget::img& Widget::render() const
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
