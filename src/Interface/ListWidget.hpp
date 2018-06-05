#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include "Widget.hpp"
#include "ButtonWidget.hpp"
#include <string>
#include <vector>

class ListWidget : public Widget
{
    public:
		explicit ListWidget(const std::vector<std::string>& texts, bool column = false, const dim_t& fontSize = { 23,0 }, const dim_t& size = { 0,0 });

    protected:
	    img actualRender() const override;
	    void actualResize(const dim_t& size) override;
	    dim_t actualSize() const override { return size_; }
	    bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		std::vector<ButtonWidget> buttons_;
		bool column_ = false;
		const dim_t fontSize_;
};
#endif // LISTWIDGET_HPP
