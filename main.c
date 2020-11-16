#include "http-server.h"
#include "resource.h"

#define PORT 80
#define DEFAULT_CACHE_DURATION 2

int main() {
    resource_t handles[3] = {0};
    handles[0] = resource_create("192.168.1.90", "temperature", "/temperature",
                                 DEFAULT_CACHE_DURATION);
    handles[1] = resource_create("192.168.1.90", "humidity", "/humidity",
                                 DEFAULT_CACHE_DURATION);
    handles[2] = resource_create("192.168.1.90", "light", "/light",
                                 DEFAULT_CACHE_DURATION);

    start_server(PORT);
    printf("Press [RETURN] to quit.\n");

    char c = getchar();
    (void)c;
    stop_server();

    for (int i = 0; i < 3; i++) {
        resource_destroy(handles[i]);
    }

    return 0;
}
