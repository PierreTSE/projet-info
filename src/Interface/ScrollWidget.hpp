#ifndef SCROLLWIDGET_HPP
#define SCROLLWIDGET_HPP

#include "Widget.hpp"


class ScrollWidget : public Widget
{
    public:
        ScrollWidget(Widget& content, dim_t size) : content_{&content}, size_{size} { content_->resize(size_); content_->setParent(this); }
    
    protected:
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        img actualRender() const override;
        bool actualPropagateEvent(const Event& event) override;

    private:
        dim_t size_;
        Widget* content_ = nullptr;
        long long delta = 0;
        
        static constexpr int scrollBarWidth = 20;
};


#endif //SCROLLWIDGET_HPP
