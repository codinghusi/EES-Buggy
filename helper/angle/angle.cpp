#include "angle.h"

Angle &operator+(float const &angle)
{
    set(value + angle);
    return *this;
}

Angle &operator+(Angle const &angle)
{
    set(value + angle.get());
    return *this;
}

Angle &operator-(float const &angle)
{
    set(value - angle);
    return *this;
}

Angle &operator-(Angle const &angle)
{
    set(value - angle.get());
    return *this;
}

Angle angleDifference(Angle a, Angle b)
{
    Angle diff(a.get() - b.get());
    if (diff.get() > 180)
        diff = diff - 360;
    else if (diff.get() < -180)
        diff = diff + 360;
    return diff;
}