#pragma once
#include <stdint.h>

#define HEADER0     (0xDEu)
#define HEADER1     (0xADu)
#define HEADER2     (0xBEu)
#define HEADER3     (0xEFu)

#define APROTO_PACKET_SIZE     (64u)

#pragma pack(push, 1)
typedef struct {
    uint8_t header[4];
    uint16_t ax, ay, az;
    uint16_t gx, gy, gz;
    uint16_t mx, my, mz;
    float q1, q2, q3, q4;
    float roll, pitch, yaw;
    uint8_t pad[14];
} aproto_t;
#pragma pack(pop)

void pack_aproto(aproto_t *packet, uint16_t ax, uint16_t ay, uint16_t az, uint16_t gx, uint16_t gy, uint16_t gz,
                 uint16_t mx, uint16_t my, uint16_t mz, float q1, float q2, float q3, float q4, float roll, float pitch, float yaw);
