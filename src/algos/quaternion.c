#include "algos/quaternion.h"

// this satisfies sqrtf's requirement of linking __errno;
volatile int __errno;

euler_angle q_toeuler(quaternion *q) {
    euler_angle res;

    res.roll = atan2f(2 * q->c * q->d - 2 * q->a * q->b, 2 * q->a * q->a + 2 * q->d * q->d - 1);
    res.pitch = -asinf(2 * q->b * q->d + 2 * q->a * q->c);
    res.yaw = atan2f(2 * q->b * q->c - 2 * q->a * q->d, 2 * q->a * q->a + 2 * q->b * q-> b - 1);

    res.roll *= (180.0f / M_PI);
    res.pitch *= (180.0f / M_PI);
    res.yaw *= (180.0f / M_PI);

    return res;
}
