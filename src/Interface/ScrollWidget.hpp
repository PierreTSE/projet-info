#ifndef SCROLLWIDGET_HPP
#define SCROLLWIDGET_HPP

#include "Widget.hpp"


class ScrollWidget : public Widget
{
    public:
        explicit ScrollWidget(Widget& content, const dim_t size) : size_{size}, content_{&content} { content_->resize(size_); content_->setParent(this); }
    
    protected:
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        img actualRender() const override;
        bool actualPropagateEvent(const Event& event) override;

    private:
        dim_t size_;
        Widget* content_ = nullptr;
        long long delta_ = 0;
		float coeff_delta_ = 20;
        mutable long barHeight_ = 0;
        
        static constexpr int scrollBarWidth = 20;
};


#endif //SCROLLWIDGET_HPP
