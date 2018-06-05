#ifndef IMAGEWIDGET_HPP
#define IMAGEWIDGET_HPP

#include "Widget.hpp"
#include "../Image/Image.hpp"
#include <chrono>


class ImageWidget : public Widget
{
    public:
        ImageWidget(const Image& image, const dim_t size) : image_{image}, size_{size} {}
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        bool actualPropagateEvent(const Event& event) override;
            
    private:
        const Image& image_; // TODO Vérifier que la référence de ne "pendouille" jamais
        dim_t size_;
        bool selected_ = false;
        std::chrono::system_clock::time_point last_clicked_;
        static constexpr std::chrono::system_clock::duration double_click_interval = std::chrono::milliseconds(250);
};


#endif //IMAGEWIDGET_HPP
