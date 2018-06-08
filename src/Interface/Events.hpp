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
	enum mouseButton_t { LEFT, MIDDLE, RIGHT };
	mouseButton_t type;
};

struct UnClickEvent
{
	enum mouseButton_t { LEFT, MIDDLE, RIGHT };
	mouseButton_t type;
};

struct ZoomEvent
{
    int amount = 0;
};

struct MoveEvent
{
    dim_t lastPos;
};




/**
 * \brief Contient une union d'�v�nements et la position de la souris quand l'un d'eux se produit
 */
struct Event
{
	dim_t pos;
	std::variant<ScrollEvent, ClickEvent, ZoomEvent, MoveEvent, UnClickEvent> event;
};

#endif //EVENTS_HPP
