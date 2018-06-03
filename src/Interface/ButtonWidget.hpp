#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <string>


class ButtonWidget : public Widget
{
    public:
		ButtonWidget(const std::string& text, const dim_t& fontSize = { 13,13 }, const dim_t& size = { 0,0 });
    
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
		const dim_t fontSize_ = { 13,13 };
};
#endif // BUTTONWIDGET_HPP
