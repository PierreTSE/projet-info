#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include "Widget.hpp"
#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp


class ListWidget : public Widget
{
    public:
        ListWidget(Collection<Image>& collec, long long width, long long minHeight, const dim_t& elemSize = {50, 50}) : // TODO remettre le const de la référence
            collec_{collec}, targetWidth_{width}, minHeight_{minHeight}, elemSize_{elemSize} {}
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
    
    private:
        mutable Collection<Image>& collec_; // TODO Enlever ce hack dégueulasse en utilisant des const_iterator
        long long targetWidth_;
        long long minHeight_;
        dim_t elemSize_;
};


#endif //LISTWIDGET_HPP
