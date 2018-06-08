#include "TagSetterWidget.hpp"
#include <algorithm>
#include "../Utilities/Utilities.hpp"


TagSetterWidget::TagSetterWidget(const TagList& possibleTags, Collection<Image>& collection, long long width, long long minHeight, const std::function<bool(ClickEvent, TagSetterWidget*, Tag)>& callback) :
    possibleTags_{possibleTags}, 
    collection_{collection}, 
    width_{width}, 
    minHeight_{minHeight},
    callback_{callback}
{

}

Widget::img TagSetterWidget::actualRender() const
{
    const auto nb_line = possibleTags_.size();
    const auto height = std::max<long long>(nb_line*lineHeight, minHeight_);
    
    img render(width_, height, 1, 3, 255);
    
    int i = 0;
    for(auto& tag : possibleTags_)
    {
        int state = 0;
        if(std::all_of(collection_.begin(), collection_.end(), [&](const Image& i) { return i.hasTag(tag); }))
            state = 2;
        else if(std::any_of(collection_.begin(), collection_.end(), [&](const Image& i) { return i.hasTag(tag); }))
            state = 1;
        if(std::distance(collection_.begin(), collection_.end()) == 0)
            state = 0;
        img line(width_, lineHeight, 1, 3, 255);
        unsigned char white[] = {255, 255, 255}, black[] = {0, 0, 0}; 
        
        line.draw_rectangle(5, 5, lineHeight-5, lineHeight-5, black);
        if(state != 2)
            line.draw_rectangle(6, 6, lineHeight-6, lineHeight-6, white);
        if(state == 1)
            line.draw_rectangle(9, 9, lineHeight-9, lineHeight-9, black);
        line.draw_text(33, (lineHeight-23)/2+1, UTF8toISO8859_1(tag).c_str(), black, white, 1, 23);
        
        render.draw_image(0, lineHeight*i, 0, 0, line);
        i++;
    }
    
    return render;
}

void TagSetterWidget::actualResize(const dim_t& size)
{
    width_ = size.x;
    minHeight_ = size.y;
    callRedraw();
}

dim_t TagSetterWidget::actualSize() const
{
    return dim_t{width_, std::max<long long>(possibleTags_.size()*lineHeight, minHeight_) };
}

bool TagSetterWidget::actualPropagateEvent(const Event& event)
{
    if(std::holds_alternative<ClickEvent>(event.event)) 
    {
        int pos = event.pos.y / lineHeight;
        auto it = possibleTags_.begin();
        bool valid = true;

        for(int i = 0; i < pos && it != possibleTags_.end(); ++i)
        {
            if(++it == possibleTags_.end())
                valid = false;
        }
            
        const auto& ce = std::get<ClickEvent>(event.event);
        if(ce.type == ClickEvent::LEFT)
        {                
            if(valid)
            {
                const Tag& tag = *it;
                bool state = std::all_of(collection_.begin(), collection_.end(), [&](const Image& i) { return i.hasTag(tag); });
                
                if(state)
                    for(auto& image : collection_)
                        image.getTagList().erase(tag);
                else
                    for(auto& image : collection_)
                        image.getTagList().insert(tag);
            }
        }
        callRedraw();
        callback_(ce, this, valid?(*it):"");
        
        return true;
    }
    
    return false;
}
