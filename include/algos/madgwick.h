#pragma once
#include "algos/quaternion.h"

#define DELTA_T     (0.03f)
#define BETA        (0.07557496f)

extern quaternion q_est;

void imu_filter(float ax, float ay, float az, float gx, float gy, float gz);
