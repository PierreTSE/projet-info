#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <variant>

struct dim_t
{
    long long x = 0;
    long long y = 0;
};

inline dim_t operator+(const dim_t& a, const dim_t& b)
{
    return dim_t{ a.x + b.x, a.y + b.y };
}

inline dim_t operator-(const dim_t& a, const dim_t& b)
{
    return dim_t{ a.x - b.x, a.y - b.y };
}

inline bool operator==(const dim_t& a, const dim_t& b)
{
    return (a.x == b.x && a.y == b.y);
}

inline bool operator!=(const dim_t& a, const dim_t& b)
{
    return !(a == b);
}

struct ScrollEvent
{
    int amount = 0;
};

struct ClickEvent
{
	dim_t pos = { 0,0 };
	enum type_t { LEFT, RIGHT };
	type_t type;
}

struct ZoomEvent
{
    int amount = 0;
};

struct MoveEvent
{
    dim_t lastPos;
};

using Event = std::pair<dim_t, std::variant<ScrollEvent, ClickEvent, ZoomEvent>>;


#endif //EVENTS_HPP
