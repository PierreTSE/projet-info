#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "../CImg.h"
#include <utility>

struct dim_t
{
	long long x;
	long long y;
};

inline dim_t operator+(const dim_t& a, const dim_t& b)
{
	return dim_t{ a.x + b.x, a.y + b.y };
}

inline dim_t operator-(const dim_t& a, const dim_t& b)
{
	return dim_t{ a.x - b.x, a.y - b.y };
}

class Widget
{
    public:
		using img = cimg_library::CImg<unsigned char>;
		const img& render() const;
		void resize(const dim_t& size);

    protected:
		virtual img actualRender() const = 0;
		virtual void actualResize(const dim_t& size) = 0;

    private:
		Widget* parent_ = nullptr;
		mutable bool needRedraw_ = true;
		mutable img cachedImg_;

};

#endif // !WIDGET_HPP

