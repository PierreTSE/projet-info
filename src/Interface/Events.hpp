#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <variant>

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

struct ScrollEvent
{
    int amount;
};

struct ClickEvent
{
    dim_t pos;
};

struct ZoomEvent
{
    int amount;
};

using Event = std::pair<dim_t, std::variant<ScrollEvent, ClickEvent, ZoomEvent>>;


#endif //EVENTS_HPP
