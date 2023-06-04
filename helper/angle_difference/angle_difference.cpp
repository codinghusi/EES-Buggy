#include "angle_difference.h"

float angleDifference(float a, float b) {
    float diff = a - b;
    if (diff > 180) {
        diff -= 360;
    } else if (diff < -180) {
        diff += 360;
    }
    return diff;
}
