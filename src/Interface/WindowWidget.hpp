#ifndef WINDOWWIDGET_HPP
#define WINDOWWIDGET_HPP

#include "Widget.hpp"


class WindowWidget : public Widget
{
    public:
        WindowWidget(Widget& content, dim_t size) : content_{&content}, size_{size} { content.setParent(this); content.resize(size); }
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override { return size_; }
        bool actualPropagateEvent(const Event& event) override;
    
    private:
        Widget* content_ = nullptr;
        dim_t size_;
        cimg_library::CimgDisplay window_;
        dim_t textPos_ = {0, 0};
        
};


#endif //WINDOWWIDGET_HPP
