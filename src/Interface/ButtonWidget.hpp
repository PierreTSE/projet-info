#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <string>


class ButtonWidget : public Widget
{
    public:
		ButtonWidget(const std::string& text, const int& fontSize = 23, const dim_t& size = { 0,0 });
    
    protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

		virtual bool execute(ClickEvent::mouseButton_t);

    private:
		dim_t size_;
		bool is_hovered = false;
		bool is_clicked = false;
		std::string text_;
		const int fontSize_;
};
#endif // BUTTONWIDGET_HPP
