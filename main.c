#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <microhttpd.h>

#include "http-server.h"
#include "resource.h"

#define PORT 80

void print_address() {
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    printf("Server started %shttp://%s:%d%s\n", "\033[92m",
           inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), PORT,
           "\033[0m");
}

int main() {
    int handles[3] = {0};
    handles[0] = resource_create("192.168.1.90", "temperature", "/temperature");
    handles[1] = resource_create("192.168.1.90", "humidity", "/humidity");
    handles[2] = resource_create("192.168.1.90", "light", "/light");

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);

    if (NULL == daemon) {
        perror("");
        return EXIT_FAILURE;
    }

    print_address();

    printf("Press [q] to quit.\n");

    char c;
    do {
        c = getchar();
    } while (c != 'q');

    MHD_stop_daemon(daemon);
    for (int i = 0; i < 3; i++) {
        resource_destroy(handles[i]);
    }

    return 0;
}
