// Communication Bus Process (comm_bus)
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

FramePart assembly_buffer;
int assembly_state = 0; // 0=empty, 1=part1, 2=part2
int station_list[MAX_STATIONS];
int log_fd;

void log_event(const char *msg) {
    dprintf(log_fd, "%s\n", msg);
    fflush(stdout);
}

int main() {
    struct sockaddr_un sock_addr;
    int socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    unlink(CBP_SOCKET_PATH);

    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, CBP_SOCKET_PATH);
    bind(socket_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

    log_fd = open("logs/comm_bus.log", O_WRONLY | O_CREAT | O_APPEND, 0666);
    log_event("Comm bus started");

    FramePart received_part;
    while (1) {
        recvfrom(socket_fd, &received_part, sizeof(received_part), 0, NULL, NULL);

        char msg[256];
        snprintf(msg, sizeof(msg), "Received part %d of frame %d from Station %d to Station %d",
            received_part.part, received_part.frame_number, received_part.src_station, received_part.dst_station);
        log_event(msg);

        if (assembly_state == 0 && received_part.part == 1) {
            memcpy(&assembly_buffer, &received_part, sizeof(received_part));
            assembly_state = 1;
        } else if (assembly_state == 1 && received_part.part == 2 &&
                   received_part.src_station == assembly_buffer.src_station &&
                   received_part.frame_number == assembly_buffer.frame_number) {
            assembly_state = 0;
            snprintf(msg, sizeof(msg), "Delivered frame %d to Station %d",
                     received_part.frame_number, received_part.dst_station);
            log_event(msg);
        } else {
            // Collision
            snprintf(msg, sizeof(msg), "Collision detected on frame %d from Station %d",
                     received_part.frame_number, received_part.src_station);
            log_event(msg);
            assembly_state = 0;
        }
    }
    return 0;
}
