#include "angle.h"

Angle Angle::operator+(const float &angle) const
{
    return Angle(value + angle);
}

Angle Angle::operator+(const Angle& angle) const
{
    return Angle(value + angle.get());
}

Angle Angle::operator-(const float& angle) const
{
    return Angle(value - angle);
}

Angle Angle::operator-(const Angle& angle) const
{
    return Angle(value - angle.get());
}

Angle &Angle::operator=(const float &angle)
{
    set(angle);
    return *this;
}

Angle angle_difference(Angle a, Angle b)
{
    Angle diff = a - b;
    if (diff.get() > 180.f)
        diff = diff - 360.f;
    else if (diff.get() < -180.f)
        diff = diff + 360.f;
    return diff;
}
