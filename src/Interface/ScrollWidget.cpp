#include "ScrollWidget.hpp"
#include <algorithm>
#include <iostream>


void ScrollWidget::actualResize(const dim_t& size)
{
	coeff_delta_ *= static_cast<double>(size.y) / size_.y;
	delta_ *= coeff_delta_;
    size_ = size;
    content_->resize(size);
    if(content_->size().y > size.y)
        content_->resize(size - dim_t{scrollBarWidth, 0});
    callRedraw();
}

dim_t ScrollWidget::actualSize() const
{
    return size_;
}

Widget::img ScrollWidget::actualRender() const
{
    img render(size_.x, size_.y, 1, 3, 255);
    if(size_.y >= content_->size().y)
    {
        // Pas de scrollbar
        render.draw_image(0, 0, 0, 0, content_->render());
    }
    else
    {
        // Avec une scrollbar
        auto temp = content_->render().get_crop(0, delta_, 0, 0, size_.x-scrollBarWidth-1, delta_+size_.y, 0, 2);
        render.draw_image(0, 0, 0, 0, temp);
        barHeight_ = static_cast<double>(size_.y) / content_->size().y * size_.y;
        long long pos = static_cast<double>(size_.y) / content_->size().y * delta_;
        render.draw_image(size_.x-scrollBarWidth, pos, 0, 0, img(scrollBarWidth, barHeight_, 1, 3, 200));
        std::cerr << delta_ << std::endl;
    }
    
    return render;
}

bool ScrollWidget::actualPropagateEvent(const Event& event)
{
	if (std::holds_alternative<ScrollEvent>(event.event))
	{
		delta_ -= std::get<ScrollEvent>(event.event).amount * coeff_delta_;
		delta_ = std::clamp<int>(delta_, 0, content_->size().y - size_.y);
		callRedraw();
	}

	//dim_t pos = event.pos;
	//std::clamp(pos.y, delta_, delta_ + barHeight_);
	//std::clamp(pos.y, 0, size_.y);
	if (isInside(event.pos - dim_t { (size_.x - scrollBarWidth), -delta_ }) && std::holds_alternative<ClickEvent>(event.event) && std::get<ClickEvent>(event.event).type == ClickEvent::LEFT)
	{
	    std::cout << "yes" << std::endl;
	}



    //vieux code
	if (content_->isInside(event.pos - dim_t {0, -delta_}))
	{
		if (content_->propagateEvent(Event{ event.pos - dim_t { 0, -delta_ }, event.event }))
			return true;
	}


    return false;
}
