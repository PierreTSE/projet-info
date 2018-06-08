#ifndef BUTTONWIDGET_HPP
#define BUTTONWIDGET_HPP

#include "Widget.hpp"
#include <functional>
#include <string>

/**
 * \class ButtonWidget
 * \brief Bouton qui lance une action configurable au clic
 * 
 * Le bouton est crée automatiquement aux bonnes dimensions depuis un texte donné à la construction.
 * Il gère différentes options comme le fait de se surligner au survol.
 * On peut lui affecter une std::function qui est exécutée au clic.
 */
class ButtonWidget : public Widget
{
    public:
        explicit ButtonWidget(const std::string& text, bool clickable = false, const int& fontSize = 23, const dim_t& size = { 0,0 });

		void setCallBack(const std::function<bool(ClickEvent, ButtonWidget*)> f) { callBack_ = f; }
		void setClickable(bool clickable) { holds_click_ = clickable; }

		const unsigned char* getBackgroundColor() const { return backgroundColor_; }
		bool isColored() const { return is_hovered_ || (holds_click_ && is_clicked_); }
    
    protected:
		img actualRender() const override;
		void actualResize(const dim_t& size) override;
		dim_t actualSize() const override { return size_; }
		bool actualPropagateEvent(const Event& event) override;

    private:
		dim_t size_;
		bool is_hovered_ = false;
		bool is_clicked_ = false;
		bool holds_click_ = false;
		std::string text_;
		const int fontSize_;
		std::function<bool(ClickEvent,ButtonWidget*)> callBack_ ;
		const unsigned char backgroundColor_[3] = { 102, 153, 255 };

		bool callBack(ClickEvent ce, ButtonWidget* bw) { return callBack_(ce, bw); }
};
#endif // BUTTONWIDGET_HPP
