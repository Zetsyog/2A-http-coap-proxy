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
    int max_cache_duration;
    int updating;
    pthread_mutex_t lock;
    time_t last_update;
};

typedef int resource_t;

/**
 *  @brief Register a new resource
 *  A resource is a struct that is used to interface an http route with a coap
 * route
 *  @param @struct resource
 *  @return the created resource
 */
resource_t resource_create(char *coap_ip, char *coap_name, char *http_route,
                    int max_cache_duration);

/**
 *  @brief Free memory by a @struct resource
 *  @param resource_t handle of the struct to destroy
 */
void resource_destroy(resource_t resource);

resource_t resource_get_by_coap(const char *coap_name);

resource_t resource_get_by_name(const char *resource_name);

/**
 *  @brief Retrieve the value of the asked resource
 *  If the cache is still fresh, the function does nothing
 *  If the cache is too old, send a request to the corresponding coap resource
 *  @param resource to update
 */
void resource_value(resource_t resource, char *dest);

void resource_value_set(resource_t resource, char *new_val, int len);

#endif