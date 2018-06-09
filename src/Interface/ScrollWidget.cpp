#include "ScrollWidget.hpp"
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

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
        const auto temp = content_->render().get_crop(0, delta_, 0, 0, size_.x-scrollBarWidth-1, delta_+size_.y, 0, 2);
        render.draw_image(0, 0, 0, 0, temp);
        const long long barHeight = static_cast<double>(size_.y) / content_->size().y * size_.y;
        const long long pos = static_cast<double>(size_.y) / content_->size().y * delta_;
        render.draw_image(size_.x-scrollBarWidth, pos, 0, 0, img(scrollBarWidth, barHeight, 1, 3, 200));
        //std::cerr << delta_ << std::endl;
    }
    
    return render;
}

bool ScrollWidget::actualPropagateEvent(const Event& event)
{

	if (std::holds_alternative<SelectEvent>(event.event))
	{
		content_->propagateEvent(event);
	}

	if (std::holds_alternative<ScrollEvent>(event.event))
	{
		delta_ -= std::get<ScrollEvent>(event.event).amount * coeff_delta_;   
		delta_ = std::clamp<int>(delta_, 0, content_->size().y - size_.y);
		callRedraw();
	}

	const long long barHeight = static_cast<double>(size_.y) / content_->size().y * size_.y;
	const long long barPos = static_cast<double>(size_.y) / content_->size().y * delta_;
	if (std::holds_alternative<ClickEvent>(event.event) && std::get<ClickEvent>(event.event).type == ClickEvent::LEFT && (event.pos.x >= (size_.x - scrollBarWidth) && event.pos.x <= size_.x && event.pos.y >= barPos && event.pos.y <= (barPos + barHeight)))
	{
		if (!followMouse_)
			followMouse_ = true;
	}
	else if(std::holds_alternative<UnClickEvent>(event.event))
	{
		if (followMouse_)
			followMouse_ = false;
	}
	else if(followMouse_ && std::holds_alternative<MoveEvent>(event.event))
	{
		delta_ += (event.pos.y - std::get<MoveEvent>(event.event).lastPos.y) * static_cast<double>(content_->size().y) / size_.y;
		delta_ = std::clamp<long long>(delta_, 0, content_->size().y - size_.y);
		callRedraw();
	}

	if (content_->isInside(event.pos - dim_t {0, -delta_}))
	{
		if (content_->propagateEvent(Event{ event.pos - dim_t { 0, -delta_ }, event.event }))
			return true;
	}


    return false;
}
