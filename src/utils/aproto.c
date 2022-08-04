#include "utils/aproto.h"


void pack_aproto(aproto_t *packet, uint16_t ax, uint16_t ay, uint16_t az, uint16_t gx, uint16_t gy, uint16_t gz,
                 uint16_t mx, uint16_t my, uint16_t mz, float q1, float q2, float q3, float q4, float roll, float pitch, float yaw) {
    // fill in the header
    packet->header[0] = HEADER0;
    packet->header[1] = HEADER1;
    packet->header[2] = HEADER2;
    packet->header[3] = HEADER3;

    // fill in accelerometer data
    packet->ax = ax;
    packet->ay = ay;
    packet->az = az;

    // fill in gyroscope data
    packet->gx = gx;
    packet->gy = gy;
    packet->gz = gz;

    // fill in magnetometer data
    packet->mx = mx;
    packet->my = my;
    packet->mz = mz;

    // fill in quaternion
    packet->q1 = q1;
    packet->q2 = q2;
    packet->q3 = q3;
    packet->q4 = q4;

    // fill in euler angles
    packet->roll = roll;
    packet->pitch = pitch;
    packet->yaw = yaw;

    // padding
    for (uint8_t idx = 0; idx < 14; idx++) {
        packet->pad[idx] = 0;
    }
}
