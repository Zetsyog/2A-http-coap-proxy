#include "resource.h"
#include "coap.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "util.h"

#define MAX_RESOURCES 10
#define ADDRESS "192.168.1.90"
#define PORT "5683"

struct resource *list[MAX_RESOURCES] = {0};
static int next = 0;

resource_t resource_get_by_name(const char *route) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (list[i] == NULL)
            continue;
        if (strcmp(list[i]->http_route, route) == 0) {
            return i;
        }
    }
    return -1;
}

resource_t resource_get_by_coap(const char *coap_name) {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (list[i] == NULL)
            continue;
        if (strcmp(list[i]->coap_name, coap_name) == 0) {
            return i;
        }
    }
    return -1;
}

int resource_create(char *coap_address, char *coap_name, char *http_route,
                    int max_cache_duration) {
    list[next] = calloc(1, sizeof(struct resource));
    struct resource *r = list[next];
    r->max_cache_duration = max_cache_duration;
    r->updating = 0;

    strcpy(r->coap_address, coap_address);
    strcpy(r->coap_name, coap_name);
    strcpy(r->http_route, http_route);
    strcpy(r->value, "NoVal");

    pthread_mutex_init(&(r->lock), NULL);

    printf("Registered route %s\n", coap_name);

    return next++;
}

void resource_destroy(resource_t handle) {
    if (list[handle] != NULL) {
        pthread_mutex_destroy(&(list[handle]->lock));
        free(list[handle]);
        list[handle] = NULL;
    }
}

struct resource *resource_get_by_id(resource_t handle) {
    if (handle < 0 || handle > MAX_RESOURCES) {
        return NULL;
    }
    return list[handle];
}

void resource_value_set(resource_t resource, char *new_val, int len) {
    if (len >= RESOURCE_DBUF_SIZE)
        return;

    struct resource *r = list[resource];

    pthread_mutex_lock(&(r->lock));
    memcpy(r->value, new_val, len);
    r->value[len] = 0;
    r->last_update = time(0);
    r->updating = 0;
    pthread_mutex_unlock(&(r->lock));
}

void resource_value(resource_t handle, char *dest) {
    struct resource *r = list[handle];

    int cache_lifetime = time(0) - r->last_update;

    if (r->max_cache_duration <= 0) {
        retrieve(r);
    } else {
        if (!r->updating) {
            if (cache_lifetime > r->max_cache_duration) {
                retrieve(r);
            } else {
                log_info("Cache still fresh for %s", r->coap_name);
            }
        } else {
            if (cache_lifetime > 2 * r->max_cache_duration) {
                log_info("[WARN] : value for route %s is obsolete",
                         r->coap_name);
                resource_value_set(handle, "NoVal", 6);
            }
        }
    }

    pthread_mutex_lock(&(r->lock));
    strcpy(dest, r->value);
    pthread_mutex_unlock(&(r->lock));
}
