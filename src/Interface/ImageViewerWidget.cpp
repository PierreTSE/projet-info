#include "ImageViewerWidget.hpp"

ImageViewerWidget::ImageViewerWidget(Image* targetImg) : 
    targetImg_{ targetImg }
{
	if (!targetImg_)
		throw std::runtime_error("ImageViewerWidget : constructor must take a valid pointer to Image.");
}


Widget::img ImageViewerWidget::actualRender() const
{
	return img();
}

void ImageViewerWidget::actualResize(const dim_t & size)
{
	size_ = size;
}

bool ImageViewerWidget::actualPropagateEvent(const Event & event)
{
	return false;
}
