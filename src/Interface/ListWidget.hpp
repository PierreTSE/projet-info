#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include "Widget.hpp"
#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"


class ListWidget : public Widget
{
    public:
        ListWidget(Collection<Image>& collec, long long width, long long minHeight, const dim_t& elemSize = {100, 100}) : // TODO remettre le const de la référence
            collec_{collec}, targetWidth_{width}, minHeight_{minHeight}, elemSize_{elemSize} {}
    
    protected:
        img actualRender() override; // TODO Remettre les const quand les const_iterator sont là
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        bool actualPropagateEvent(const Event& event) override;
    
    private:
        Collection<Image>& collec_;
        long long targetWidth_;
        long long minHeight_;
        dim_t elemSize_;
};


#endif //LISTWIDGET_HPP
