#pragma once
#include <math.h>

#ifndef M_PI
    #define M_PI    (3.14159265f)
#endif

// a + bi + cj + dk
typedef struct {
    float a;
    float b;
    float c;
    float d;
} quaternion;

typedef struct {
    float roll;
    float pitch;
    float yaw;
} euler_angle;

/**
 * Multiplies quaternion by a scalar.
 *
 * @param q Pointer to a quaternion.
 * @param s Scalar to multiply by.
 *
 * @return res Quaternion containing the result (s * <q>).
 */
static inline quaternion q_scalar_mult(quaternion *q, float s) {
    quaternion res;

    res.a = q->a * s;
    res.b = q->b * s;
    res.c = q->c * s;
    res.d = q->d * s;
    
    return res;
}

/**
 * Adds two quaternions.
 *
 * @param x Pointer to the first quaternion.
 * @param y Pointer to the second quaternion.
 *
 * @return res Quaternion containing the result (<x> + <y>).
 */
static inline quaternion q_sum(quaternion *x, quaternion *y) {
    quaternion res;

    res.a = x->a + y->a;
    res.b = x->b + y->b;
    res.c = x->c + y->c;
    res.d = x->d + y->d;

    return res;
}

/**
 * Subtracts two quaternions.
 *
 * @param x Pointer to the first quaternion.
 * @param y Pointer to the second quaternion.
 *
 * @return res Quaternion containing the result (<x> - <y>).
 */
static inline quaternion q_sub(quaternion *x, quaternion *y) {
    quaternion res;

    res.a = x->a - y->a;
    res.b = x->b - y->b;
    res.c = x->c - y->c;
    res.d = x->d - y->d;

    return res;
}

/**
 * Conjugate of the quaternion.
 *
 * @param q Pointer to a quaternion.
 *
 * @return res Quaternion containing the result (<q>*).
 */
static inline quaternion q_conj(quaternion *q) {
    quaternion res;

    res.a = q->a;
    res.b = -q->b;
    res.c = -q->c;
    res.d = -q->d;

    return res;
}

/**
 * Norm of the quaternion.
 *
 * @param q Pointer to a quaternion.
 *
 * @return res Float containing the result (||<q>||).
 */
static inline float q_norm(quaternion *q) {
    return sqrtf(q->a * q->a + q->b * q->b + q->c * q->c + q->d * q->d);
}

/**
 * Normalizes a quaternion.
 *
 * @param q Pointer to a quaternion.
 *
 * @return res Float containing the result (<q> / ||<q>||).
 */
static inline quaternion q_normalize(quaternion *q) {
    quaternion res;
    float norm;
    
    // get the norm of q
    norm = q_norm(q);

    // normalize q
    res.a = q->a / norm;
    res.b = q->b / norm;
    res.c = q->c / norm;
    res.d = q->d / norm;

    return res;
}

/**
 * Multiplies two quaternions.
 *
 * @param x Pointer to the first quaternion.
 * @param y Pointer to the second quaternion.
 *
 * @return res Quaternion containing the result (<x> * <y>).
 */
static inline quaternion q_mult(quaternion *x, quaternion *y) {
    quaternion res;

    res.a = (x->a * y->a) - (x->b * y->b) - (x->c * y->c) - (x->d * y->d);
    res.b = (x->a * y->b) + (x->b * y->a) + (x->c * y->d) - (x->d * y->c);
    res.c = (x->a * y->c) - (x->b * y->d) + (x->c * y->a) + (x->d * y->b);
    res.d = (x->a * y->d) + (x->b * y->c) - (x->c * y->b) + (x->d * y->a);
    
    return res;
}

/**
 * Converts a quaternion to euler angles. Best used for debug.
 *
 * @param q Pointer to a quaternion.
 *
 * @return res Struct containing roll/pitch/yaw.
 */
euler_angle q_toeuler(quaternion *q);
