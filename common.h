#ifndef COMMON_H
#define COMMON_H

#define MAX_STATIONS 10
#define MAX_FRAME_SIZE 1024
#define SLOT_DURATION_MS 100
#define MAX_COLLISIONS 16
#define CBP_SOCKET_PATH "/tmp/ethernet_cbp.sock"

typedef struct {
    int src_station;
    int dst_station;
    int frame_number;
    int part; // 1 or 2
    char payload[256];
} FramePart;

#endif
