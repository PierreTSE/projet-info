#ifndef IMAGEVIEWERWIDGET_HPP
#define IMAGEVIEWERWIDGET_HPP
#include "../Image/Image.hpp"
#include "Widget.hpp"

/**
 * \brief Gère une liste de Tags sans interactions
 */
class TagViewerWidget : public Widget
{
    public:
		explicit TagViewerWidget(Image* targetImg, long long width, long long minHeight);
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override;
		bool actualPropagateEvent(const Event& event) override;

    private:
		Image* targetImg_ = nullptr;
		long long width_;
		long long minHeight_;

		static constexpr long long lineHeight = 30;
};
#endif // IMAGEVIEWERWIDGET_HPP
