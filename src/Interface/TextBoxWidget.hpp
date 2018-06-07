#ifndef TEXTBOXWIDGET_HPP
#define TEXTBOXWIDGET_HPP

#include "Widget.hpp"
#include <string>


class TextBoxWidget : public Widget
{
    public:
		explicit TextBoxWidget(size_t fontSize = 23);
		img actualRender() const override;
        void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		size_t fontSize_;
		std::string text_;
		bool is_clicked_ = false;
};

#endif // TEXTBOXWIDGET_HPP
