#ifndef TAGSETTERWIDGET_HPP
#define TAGSETTERWIDGET_HPP

#include "Widget.hpp"
#include "../Collection/Collection.hpp"
#include "../Image/Image.hpp"


class TagSetterWidget : public Widget
{
    public:
        TagSetterWidget(const TagList& possibleTags, Collection<Image>& collection, long long width, long long minHeight);
    
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
        
        static constexpr int lineHeight = 20;
};


#endif //TAGSETTERWIDGET_HPP
