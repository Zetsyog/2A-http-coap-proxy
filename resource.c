#include "resource.h"
#include "coap.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"

#define MAX_RESOURCES 10
#define ADDRESS "192.168.1.90"
#define PORT "5683"
#define CACHE_DURATION 10

struct resource *list[MAX_RESOURCES] = {0};
static int next = 0;

int resource_get(const char *route) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if(list[i] == NULL) continue;
        if (strcmp(list[i]->http_route, route) == 0) {
            return i;
        }
    }
    return -1;
}

int resource_get_by_coap(const char *coap_name) {
    int ret = -1;
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if(list[i] == NULL) continue;
        pthread_mutex_lock(&(list[i]->mutex));
        if (strcmp(list[i]->coap_name, coap_name) == 0) {
            ret = i;
        }
        pthread_mutex_unlock(&(list[i]->mutex));
        if(ret != -1) break;
    }
    return ret;
}

int resource_create(char *coap_address, char *coap_name, char *http_route) {
    list[next] = calloc(1, sizeof(struct resource));
    struct resource *r = list[next];

    strcpy(r->coap_address, coap_address);
    strcpy(r->coap_name, coap_name);
    strcpy(r->http_route, http_route);
    strcpy(r->value, "NoVal");

    pthread_mutex_init(&(r->mutex), NULL);

    printf("Registered route %s\n", coap_name);

    return next++;
}

void resource_destroy(int handle) {
    if (list[handle] != NULL) {
        pthread_mutex_destroy(&(list[handle]->mutex));
        free(list[handle]);
        list[handle] = NULL;
    }
}

struct resource *resource_get_by_id(int handle) {
    if(handle < 0 || handle > MAX_RESOURCES) {
        return NULL;
    }
    return list[handle];
}

void resource_value(int handle, char *dest) {
    struct resource *r = list[handle];

    if (time(0) - r->last_update > 10) {
        log_info("[COAP] Get: %s", r->coap_name);
        retrieve(r);
    } else {
        log_info("Cache still fresh for %s", r->coap_name);
    }

    pthread_mutex_lock(&(r->mutex));
    strcpy(dest, r->value);
    pthread_mutex_unlock(&(r->mutex));
}
