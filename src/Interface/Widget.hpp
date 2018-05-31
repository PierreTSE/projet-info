#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "../CImg.h"
#include "Events.hpp"
#include <utility>


/**
 * \class Widget
 * \brief API permettant la création d'une interface graphique
 * 
 * Cette classe représente le concept de Widget, qui constitue la brique élémentaire d'une interface graphique.
 * TODO doc
 */
class Widget
{
    public:
		using img = cimg_library::CImg<unsigned char>;
		const img& render() const;
		void resize(const dim_t& size);
		dim_t size() const;
		bool propagateEvent(const Event& event);
		bool isInside(const dim_t& pos) const;
		void setParent(Widget* wid) { parent_ = wid; }
		virtual ~Widget() = default;

	protected:
		virtual img actualRender() const = 0;
		virtual void actualResize(const dim_t& size) = 0;
		virtual dim_t actualSize() const = 0;
		virtual bool actualPropagateEvent(const Event& event) = 0;
		virtual bool actualIsInside(const dim_t& pos) const;

        void needRedraw() const { needRedraw_ = true; if(parent_) parent_->needRedraw(); };

		Widget * parent_ = nullptr;
		mutable bool needRedraw_ = true;

    private:		
		mutable img cachedImg_;

};

#endif // !WIDGET_HPP

