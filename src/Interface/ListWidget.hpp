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

/**
 * \class ListWidget
 * \brief Gère un menu de \c ButtonWidget
 * Le menu peut être affiché en ligne où en colonne.
 * La taille est gérée selon la hauteur des boutons en ligne ;
 * selon la largeur du plus grand bouton en colonne.
 */
class ListWidget : public Widget
{
    public:
        explicit ListWidget(const std::vector<std::string>& texts, bool column = false, int fontSize = 23, const dim_t& size = { 0,0 });
        ListWidget(const ListWidget& other);

		void setCallBack(size_t i, const std::function<bool(ClickEvent, ButtonWidget*)>& f = [](ClickEvent ce, ButtonWidget* be) {return true; }) { buttons_.at(i).setCallBack(f); }
		size_t getLength() const { return buttons_.size(); }
        
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
