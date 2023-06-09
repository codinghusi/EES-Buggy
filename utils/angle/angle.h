#pragma once

#include <cmath>

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

Angle angleDifference(Angle a, Angle b);
