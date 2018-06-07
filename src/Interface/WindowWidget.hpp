#ifndef WINDOWWIDGET_HPP
#define WINDOWWIDGET_HPP

#include <memory>
#include "Widget.hpp"


class WindowWidget : public Widget
{
    public:
        WindowWidget(Widget* content, dim_t size);
    
        void manageEvents();
        bool is_open() const { return !window_.is_closed(); }
        void display() { window_.display(render()); }
        void setContent(Widget* content);
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override { return size_; }
        bool actualPropagateEvent(const Event& event) override;

        WindowWidget* getWindow() override { return this; }
        const WindowWidget* getWindow() const override { return this; }
    
    private:
        std::unique_ptr<Widget> content_ = nullptr;
        dim_t size_;
        cimg_library::CImgDisplay window_;
        dim_t textPos_ = {0, 0};
        dim_t lastMouse_;
        int lastWheel_;
        unsigned int lastButton_;
        
};


#endif //WINDOWWIDGET_HPP
