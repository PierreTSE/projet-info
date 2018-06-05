#ifndef MENUBARWIDGET_HPP
#define MENUBARWIDGET_HPP

#include "Widget.hpp"
#include "ListWidget.hpp"
#include <vector>


class MenuBarWidget :public Widget
{
    public:
		MenuBarWidget() = default;

    protected:
        img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
        bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		Widget* content_ = nullptr;
		std::vector<ButtonWidget> buttons_;
};

#endif // MENUBARWIDGET_HPP