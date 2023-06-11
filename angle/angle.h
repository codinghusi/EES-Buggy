#pragma once

#include <cmath>


/**
 * Wraps an angle value to be between -360 and 360 degrees.
 * Can perform simple operations on angles.
*/
class Angle{
    float value;
public:
    Angle(float angle){ set(angle); }

    void set(float angle) { value = std::fmod(angle, 360.f); }
    float get() const { return value; }

    Angle operator+(const float& angle) const;
    Angle operator+(const Angle& angle) const;
    Angle operator-(const float& angle) const;
    Angle operator-(const Angle& angle) const;
    Angle& operator=(const float& angle);
};

/**
 * Difference between two angles: a - b.
 * Returns an angle between -180 and 180 degrees.
*/
Angle angle_difference(Angle a, Angle b);
