// STATION PROCESS
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <time.h>

void wait_slot(int slots) {
    usleep(slots * SLOT_DURATION_MS * 1000);
}

void log_event(int id, const char *msg) {
    char filename[64];
    snprintf(filename, sizeof(filename), "logs/station%d.log", id);
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
    dprintf(fd, "%s\n", msg);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <station_id> <input_file>\n", argv[0]);
        exit(1);
    }

    int id = atoi(argv[1]);
    FILE *f = fopen(argv[2], "r");
    if (!f) exit(1);

    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, CBP_SOCKET_PATH);

    int frame = 1, collision_count = 0;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        int dst;
        sscanf(line, "Frame %*d, To Station %d", &dst);

        FramePart p1 = {id, dst, frame, 1, "Data1"};
        FramePart p2 = {id, dst, frame, 2, "Data2"};

        log_event(id, "Send part 1");
        sendto(sockfd, &p1, sizeof(p1), 0, (struct sockaddr*)&addr, sizeof(addr));

        wait_slot(1);

        if (rand() % 5 == 0) { // simulate collision randomly
            log_event(id, "Collision detected, retrying");
            int backoff = rand() % (1 << (collision_count < 10 ? collision_count : 10));
            wait_slot(backoff);
            collision_count++;
            continue;
        }

        log_event(id, "Send part 2");
        sendto(sockfd, &p2, sizeof(p2), 0, (struct sockaddr*)&addr, sizeof(addr));
        frame++;
        collision_count = 0;
        wait_slot(1);
    }
    fclose(f);
    return 0;
}