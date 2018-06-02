#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <string>


class ButtonWidget : public Widget
{
    public:
		ButtonWidget(Widget& content, const dim_t& size, const std::string& text);
    
    protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		Widget* content_ = nullptr;
		bool is_hovered = false;
		bool is_clicked = false;
		std::string text_;
		const size_t fontSize_ = 13;
};
#endif // BUTTONWIDGET_HPP
