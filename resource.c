#include "resource.h"
#include "string.h"
#include "coap.h"
#include "stdio.h"
#include "util.h"
#include "stdlib.h"

#define MAX_RESOURCES 10
#define ADDRESS "192.168.1.90"
#define PORT "5683"
#define CACHE_DURATION 10

struct resource *list;
static int next = 0;

void resource_init() {
    list = calloc(MAX_RESOURCES, sizeof(struct resource));
}

void resource_dispose() {
    free(list);
}

int resource_get(const char *route) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(list[i].http_route, route) == 0) {
            return i;
        }
    }
    return -1;
}

int resource_get_by_coap(const char *coap_name) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (strcmp(list[i].coap_name, coap_name) == 0) {
            return i;
        }
    }
    return -1;
}

int resource_register(char *coap_address, char *coap_name, char *http_route) {
    struct resource *r = &list[next];

    strcpy(r->coap_address, coap_address);
    strcpy(r->coap_name, coap_name);
    strcpy(r->http_route, http_route);
    strcpy(r->value, "NoVal");

    printf("Registered route %s\n", coap_name);

    return next++;;
}

struct resource *resource_get_by_id(int handle) {
    return &list[handle];
}

char *resource_value(int handle) {
    struct resource *r = &list[handle];
    if(time(0) - r->last_update > 10) {
        retrieve(r);
        log_info("[COAP] Get: %s", r->coap_name);
    }

    return r->value;
}
