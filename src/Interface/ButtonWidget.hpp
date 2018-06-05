#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <functional>
#include <string>


class ButtonWidget : public Widget
{
    public:
        explicit ButtonWidget(const std::string& text, const int& fontSize = 23, const dim_t& size = { 0,0 });

  //      template<typename R = bool, typename... Args>
		//void setCallBack(std::function<R(Args...)> f) { callBack_<Args...> = f; }
    
    protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

		virtual bool execute(ClickEvent::mouseButton_t);

    private:
		dim_t size_;
		bool is_hovered_ = false;
		bool is_clicked_ = false;
		std::string text_;
		const int fontSize_;

		//template<typename R = bool, typename... Args>
		//std::function<R(Args...)> callBack_ ;

		//template<typename R = bool, typename... Args>
		//bool callBack(Args... args) { return callBack_(args); }

};
#endif // BUTTONWIDGET_HPP
