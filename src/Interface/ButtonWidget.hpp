#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <functional>
#include <string>


class ButtonWidget : public Widget
{
    public:
        explicit ButtonWidget(const std::string& text, const int& fontSize = 23, const dim_t& size = { 0,0 });

		void setCallBack(const std::function<bool(ClickEvent, ButtonWidget*)> f) { callBack_ = f; }
    
    protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		bool is_hovered_ = false;
		bool is_clicked_ = false;
		std::string text_;
		const int fontSize_;
		std::function<bool(ClickEvent,ButtonWidget*)> callBack_ ;

		bool callBack(ClickEvent ce, ButtonWidget* bw) { return callBack_(ce, bw); }

};
#endif // BUTTONWIDGET_HPP
