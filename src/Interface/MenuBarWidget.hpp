#ifndef MENUBARWIDGET_HPP
#define MENUBARWIDGET_HPP

#include "Widget.hpp"
#include "ListWidget.hpp"
#include <vector>
#include <memory>


class MenuBarWidget :public Widget
{
	public:
		MenuBarWidget(Widget* content, ListWidget* buttons, const dim_t& size);
	
	protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;
	
	private:
		dim_t size_;
		std::unique_ptr<Widget> content_ = nullptr;
		std::unique_ptr<ListWidget> buttons_ = nullptr;
};

#endif // MENUBARWIDGET_HPP