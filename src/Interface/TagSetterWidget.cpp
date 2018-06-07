#include "TagSetterWidget.hpp"
#include <algorithm>



TagSetterWidget::TagSetterWidget(const TagList& possibleTags, Collection<Image>& collection, long long width, long long minHeight) :
    possibleTags_{possibleTags}, 
    collection_{collection}, 
    width_{width}, 
    minHeight_{minHeight}
{

}

Widget::img TagSetterWidget::actualRender() const
{
    auto nb_line = possibleTags_.size();
    auto height = std::max<long long>(nb_line*lineHeight, minHeight_);
    
    img render(width_, height, 1, 3, 255);
    
    int i = 0;
    for(auto& tag : possibleTags_)
    {
        int state = 0;
        if(std::all_of(collection_.begin(), collection_.end(), [&](const Image& i) { return i.hasTag(tag); }))
            state = 2;
        else if(std::any_of(collection_.begin(), collection_.end(), [&](const Image& i) { return i.hasTag(tag); }))
            state = 1;
        img line(width_, lineHeight, 1, 3, 255);
        unsigned char white[] = {255, 255, 255};
        unsigned char black[] = {0, 0, 0}; 
        
        line.draw_rectangle(5, 5, lineHeight-5, lineHeight-5, black);
        if(state != 2)
            line.draw_rectangle(6, 6, lineHeight-6, lineHeight-6, white);
        if(state == 1)
            line.draw_rectangle(8, 8, lineHeight-8, lineHeight-8, black);
        line.draw_text(30, (lineHeight-13)/2, tag.c_str(), black, white);
        
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
    return dim_t{width_, minHeight_}; // TODO À changer
}

bool TagSetterWidget::actualPropagateEvent(const Event& event)
{
    if(std::holds_alternative<ClickEvent>(event.event)) 
    {
        ClickEvent& ce = std::get<ClickEvent>(event.event);
        if(ce.type == ClickEvent::LEFT)
        {
            
        }
        
        return true;
    }
    
    return false;
}
