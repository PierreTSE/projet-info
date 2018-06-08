#ifndef SCROLLWIDGET_HPP
#define SCROLLWIDGET_HPP

#include "Widget.hpp"
#include <memory>


/**
 * \class ScrollWidget
 * \brief Ascenseur permettant de déplacer le contenu du Widget associé
 */
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
        float coeff_delta_ = 40;
		bool followMouse_ = false;
        
        static constexpr int scrollBarWidth = 20;
};


#endif //SCROLLWIDGET_HPP
