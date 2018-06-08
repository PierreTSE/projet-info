#ifndef IMAGEWIDGET_HPP
#define IMAGEWIDGET_HPP

#include "Widget.hpp"
#include "Events.hpp"
#include "../Image/Image.hpp"
#include <chrono>
#include <functional>


class ImageWidget : public Widget
{
    public:
        ImageWidget(Image& image, const dim_t size, const std::function<bool(ClickEvent, ImageWidget*)>& callback = [](ClickEvent, ImageWidget*){ return false;}) : 
            image_{image}, size_{size}, callback_{callback} {}
        Image& getImage() { return image_; }
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        bool actualPropagateEvent(const Event& event) override;
            
    private:
        Image& image_; // TODO Vérifier que la référence de ne "pendouille" jamais
        dim_t size_;
        std::chrono::system_clock::time_point last_clicked_;
        std::function<bool(ClickEvent, ImageWidget*)> callback_ = [](ClickEvent, ImageWidget*){ return false;};
        
        static constexpr std::chrono::system_clock::duration double_click_interval = std::chrono::milliseconds(250);
};


#endif //IMAGEWIDGET_HPP
