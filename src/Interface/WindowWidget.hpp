#ifndef WINDOWWIDGET_HPP
#define WINDOWWIDGET_HPP

#include "Widget.hpp"
#include "ListWidget.hpp"
#include <memory>
#include <functional>

class WindowWidget : public Widget
{
    public:
        WindowWidget(Widget* content, dim_t size);
    
        void manageEvents();
        bool is_open() const { return !window_.is_closed(); }
        void display() { window_.display(render()); }
        void setContent(Widget* content);
		void setCallBack(const std::function<bool(bool)>& f) { callBack_ = f; }
        
        void spawnRightClickMenu(ListWidget* rightClickMenu, dim_t pos = {-1, -1});
		void close() { window_.close(); }
    
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
        
        bool needRightClickMenu_;
        bool isRightMenuActive_ = false;
        dim_t rightClickMenuPos_ = {0, 0};
        std::unique_ptr<ListWidget> rightClickMenu_;
		std::function<bool(bool)> callBack_;
};


#endif //WINDOWWIDGET_HPP
