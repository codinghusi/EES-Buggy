#include <cmath>

class Angle{
    float value;
public:
    Angle(float angle){ set(angle); }

    void set(float angle){ value = std::fmodf(angle, 360); }
    float get(){ return value; }

    Angle& operator+(float const& angle);
    Angle &operator+(Angle const &angle);
    Angle &operator-(float const &angle);
    Angle &operator-(Angle const &angle);
};

Angle angleDifference(Angle a, Angle b);