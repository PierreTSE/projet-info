#ifndef IMAGEVIEWERWIDGET_HPP
#define IMAGEVIEWERWIDGET_HPP
#include "../Image/Image.hpp"
#include "Widget.hpp"

class ImageViewerWidget : public Widget
{
    public:
		explicit ImageViewerWidget(Image* targetImg);
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_;}
		bool actualPropagateEvent(const Event& event) override;

    private:
		Image* targetImg_ = nullptr;
		dim_t size_;
};
#endif // IMAGEVIEWERWIDGET_HPP
