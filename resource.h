#ifndef RESOURCES_H
#define RESOURCES_H

#include <pthread.h>
#include <time.h>

#define STR_LEN 100

struct resource {
    char coap_name[STR_LEN];
    char http_route[STR_LEN];
    char value[STR_LEN];
    char coap_address[16];
    int last_cache;
    pthread_mutex_t mutex;
    time_t last_update;
};

/**
 *  @brief Register a new resource
 *  A resource is a struct that is used to interface an http route with a coap
 * route
 *  @param @struct resource
 */
int resource_create(char *coap_ip, char *coap_name, char *http_route);
void resource_destroy(int handle);

int resource_get(const char *resource_name);

struct resource *resource_get_by_id(int handle);

int resource_get_by_coap(const char *coap_name);

/**
 *  @brief Retrieve the value of the asked resource
 *  If the cache is still fresh, the function does nothing
 *  If the cache is too old, send a request to the corresponding coap resource
 *  @param resource_handle The handle to the resource where to get the value
 *  @return
 */
void resource_value(int resource_handle, char *dest);

#endif