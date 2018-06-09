#ifndef GRIDWIDGET_HPP
#define GRIDWIDGET_HPP

#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "../Collection/Collection.hpp"
#include "../Collection/FilteredCollection.hpp"
#include <map>


/**
 * \brief Gère une grille d' \c ImageWidget
 * 
 * Il gère le zoom.
 */
class GridWidget : public Widget
{
    public:
        GridWidget(Collection<Image>& collec, long long width, long long minHeight, const dim_t& elemSize = {100, 100},
                   const std::function<bool(ClickEvent, ImageWidget*)>& callback = [](ClickEvent, ImageWidget*){ return false;}) :
            collec_{collec}, targetWidth_{width}, minHeight_{minHeight}, elemSize_{elemSize}, callback_{callback} {
            for(auto& image : collec_)
            {
                ImageWidget temp(image, elemSize_, callback_);
                temp.setParent(this);
                associatedWidgets_.insert({image.getPath(), std::move(temp)});
            }
        }
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        bool actualPropagateEvent(const Event& event) override;
    
    private:
        Collection<Image>& collec_;
        mutable std::map<std::experimental::filesystem::path, ImageWidget> associatedWidgets_;
        long long targetWidth_;
        long long minHeight_;
        dim_t elemSize_;
        std::function<bool(ClickEvent, ImageWidget*)> callback_;
};

#endif //GRIDWIDGET_HPP
