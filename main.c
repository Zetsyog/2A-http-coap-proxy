#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <stdlib.h>

#include <fcntl.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "http-server.h"
#include "resource.h"

#define PORT 80

int main() {
    resource_register("192.168.1.90", "temperature", "/temperature");
    resource_register("192.168.1.90", "humidity", "/humidity");
    resource_register("192.168.1.90", "light", "/light");

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        perror("");
        return EXIT_FAILURE;
    }

    printf("Server started %shttp://127.0.0.1:%d%s\n", "\033[92m", PORT,
           "\033[0m");
    printf("Press [q] to quit.\n");

    char c;
    do 
    {
        c = getchar();
    } while (c != 'q');

    MHD_stop_daemon(daemon);

    return 0;
}
