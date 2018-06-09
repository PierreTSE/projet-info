#ifndef LAYOUTWIDGET_HPP
#define LAYOUTWIDGET_HPP

#include "Widget.hpp"
#include <memory>


/**
 * \brief G�re deux \c Widget dispos�s c�te � c�te
 */
class LayoutWidget : public Widget
{
    public:
        LayoutWidget(Widget* contentLeft, Widget* contentRight, float ratio, dim_t size);
            
        protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override { return size_; }
        bool actualPropagateEvent(const Event& event) override;
        
    private:
        std::unique_ptr<Widget> contentLeft_;
        std::unique_ptr<Widget> contentRight_;
        float ratio_;
        dim_t size_;
};


#endif //LAYOUTWIDGET_HPP
