#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include "Widget.hpp"

class ListWidget : public Widget
{
    public:
		ListWidget() = default;

    protected:
	    img actualRender() const override;
	    void actualResize(const dim_t& size) override;
	    dim_t actualSize() const override { return size_; }
	    bool actualPropagateEvent(const Event& event) override;
	    bool actualIsInside(const dim_t& pos) const override;


};
#endif // LISTWIDGET_HPP
