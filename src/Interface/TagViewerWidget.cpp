#include "TagViewerWidget.hpp"
#include "../Utilities/Utilities.hpp"


TagViewerWidget::TagViewerWidget(Image* targetImg, long long width, long long minHeight) :
    targetImg_{ targetImg },
	width_ {width},
    minHeight_{ minHeight }
{
	if (!targetImg_)
		throw std::runtime_error("TagViewerWidget : constructor must take a valid pointer to Image.");
}


Widget::img TagViewerWidget::actualRender() const
{
	const auto nb_line = targetImg_->getTagList().size();
	const auto height = std::max<long long>(nb_line*lineHeight, minHeight_);

	img render(width_, height, 1, 3, 255);
    
	int i = 0;
	for (auto& tag : targetImg_->getTagList())
	{
		img line(width_, lineHeight, 1, 3, 255);
		unsigned char white[] = { 255, 255, 255 }, black[] = { 0, 0, 0 };

		line.draw_text(5, (lineHeight - 23) / 2, UTF8toISO8859_1(tag).c_str(), black, white, 1, 23);

		render.draw_image(0, lineHeight*i, 0, 0, line);
		i++;
	}

	return render;
}

void TagViewerWidget::actualResize(const dim_t & size)
{
	width_ = size.x;
	minHeight_ = size.y;
	callRedraw();
}

dim_t TagViewerWidget::actualSize() const
{
    return dim_t{ width_, std::max<long long>(targetImg_->getTagList().size(), minHeight_) };
}

bool TagViewerWidget::actualPropagateEvent(const Event & event)
{
	return false;
}
