#ifndef TAGSETTERWIDGET_HPP
#define TAGSETTERWIDGET_HPP

#include <functional>
#include "Widget.hpp"
#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"


/**
 * \brief Gère une liste de Tags pour modifier ceux des \c Images
 */
class TagSetterWidget : public Widget
{
    public:
        TagSetterWidget(const TagList& possibleTags, Collection<Image>& collection, long long width, long long minHeight, const std::function<bool(ClickEvent, TagSetterWidget*, Tag)>& callback = [](ClickEvent, TagSetterWidget*, Tag){ return false; });
    
    protected:
        img actualRender() const override;
        void actualResize(const dim_t& size) override;
        dim_t actualSize() const override;
        bool actualPropagateEvent(const Event& event) override;
    
    private:
        const TagList& possibleTags_;
        Collection<Image>& collection_;
        long long width_;
        long long minHeight_;
        std::function<bool(ClickEvent, TagSetterWidget*, Tag)> callback_;
        
        static constexpr int lineHeight = 30;
};


#endif //TAGSETTERWIDGET_HPP
