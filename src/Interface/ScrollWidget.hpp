#ifndef SCROLLWIDGET_HPP
#define SCROLLWIDGET_HPP

#include "Widget.hpp"
#include <memory>


class ScrollWidget : public Widget
{
    public:
        ScrollWidget(Widget* content, dim_t size) : content_{content}, size_{size} { content_->resize(size_); content_->setParent(this); }
    
    protected:
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        img actualRender() const override;
        bool actualPropagateEvent(const Event& event) override;

    private:
        dim_t size_;
        std::unique_ptr<Widget> content_ = nullptr;
        long long delta_ = 0;
        float coeff_delta_ = 20;
        mutable long barHeight_ = 0;
        
        static constexpr int scrollBarWidth = 20;
};


#endif //SCROLLWIDGET_HPP
