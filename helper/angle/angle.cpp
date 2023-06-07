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

Angle angleDifference(Angle a, Angle b)
{
    Angle diff(a.get() - b.get());
    if (diff.get() > 180)
        diff = diff - 360;
    else if (diff.get() < -180)
        diff = diff + 360;
    return diff;
}
