#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"


class ButtonWidget : public Widget
{
    public:
		ButtonWidget(Widget* content, dim_t size) : content_{ content }, size_{ size } { content_->resize(size_); content_->setParent(this); }

    private:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

		dim_t size_;
		Widget* content_ = nullptr;
        

};
#endif // BUTTONWIDGET_HPP
