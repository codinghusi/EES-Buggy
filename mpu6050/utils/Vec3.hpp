#pragma once
#include <math.h>
#include <iostream>

template<typename T>
struct Vec3 {
    T x = 0, y = 0, z = 0;

    Vec3<T> operator*(float lambda) {
        return Vec3{
            x * lambda,
            y * lambda,
            z * lambda
        };
    }

    Vec3<T> operator/(float lambda) {
        return Vec3{
            x / lambda,
            y / lambda,
            z / lambda
        };
    }

    Vec3<T> operator+(Vec3<T> v2) {
        return Vec3{
            x + v2.x,
            y + v2.y,
            z + v2.z
        };
    }

    Vec3<T> operator-(Vec3<T> v2) {
        return Vec3{
            x - v2.x,
            y - v2.y,
            z - v2.z
        };
    }

    float abs() {
        return sqrt(x*x + y*y + z*z);
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
