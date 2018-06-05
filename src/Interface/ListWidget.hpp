#ifndef LISTWIDGET_HPP
#define LISTWIDGET_HPP

#include "Widget.hpp"
#include "ButtonWidget.hpp"
#include <string>
#include <vector>

template <typename T, typename U>
struct CoordinatesMap
{
	std::vector<T> x;
	std::vector<U> y;
};

class ListWidget : public Widget
{
    public:
        explicit ListWidget(const std::vector<std::string>& texts, bool column = false, int fontSize = 23, const dim_t& size = { 0,0 });

    protected:
	    img actualRender() const override;
	    void actualResize(const dim_t& size) override;
	    dim_t actualSize() const override { return size_; }
	    bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		std::vector<ButtonWidget> buttons_;
		bool column_ = false;
		const int fontSize_;
		bool is_hovered_ = false;
		CoordinatesMap<int, int> buttonCoordinates_ = { {0},{0} };
};
#endif // LISTWIDGET_HPP
