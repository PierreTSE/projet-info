#ifndef MENUBARWIDGET_HPP
#define MENUBARWIDGET_HPP

#include "Widget.hpp"
#include <vector>


class ButtonWidget;

class MenuBarWidget :public Widget
{
    public:
		MenuBarWidget() = default;

    private:
        img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
        bool actualPropagateEvent(const Event& event) override;

		dim_t size_;
		Widget* content_ = nullptr;
		size_t delta;
		std::vector<ButtonWidget> buttons_;
};

#endif // MENUBARWIDGET_HPP