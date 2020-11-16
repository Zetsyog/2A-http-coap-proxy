#ifndef RESOURCES_H
#define RESOURCES_H

#include <pthread.h>
#include <time.h>

#define RESOURCE_DBUF_SIZE 100
#define RESOURCE_NAME_SIZE 100

struct resource {
    char coap_name[RESOURCE_NAME_SIZE];
    char http_route[RESOURCE_NAME_SIZE];
    char value[RESOURCE_DBUF_SIZE];
    char coap_address[16];
    int max_cache_duration;
    int updating;
    pthread_mutex_t lock;
    time_t last_update;
};

typedef int resource_t;

/**
 * @brief Register a new resource
 * A resource is a struct that is used to interface an http route with a coap
 * route
 * @param ip The IP address of the coap endpoint
 * @param name The URI of the coap resource
 * @param route The name of the HTTP route
 * @param max_cache_duration The validity duration of the cache
 * @return The created resource
 */
resource_t resource_create(char *coap_ip, char *coap_name, char *http_route,
                    int max_cache_duration);

/**
 *  @brief Free memory of a @struct resource
 *  @param handle The handle of the struct to destroy
 */
void resource_destroy(resource_t resource);

/**
 * Search a resource handle from a given coap name
 * @param name The name of the coap resource
 * @return The resource handle if found, -1 on error 
 */
resource_t resource_get_by_coap(const char *coap_name);

/**
 * Search a resource handle from a given HTTP name
 * @param name The name of the http resource
 * @return The resource handle if found, -1 on error 
 */
resource_t resource_get_by_name(const char *resource_name);

/**
 * @brief Retrieve the value of the asked resource
 * If the cache is still fresh, the function does nothing
 * If the cache is too old, send a request to the corresponding coap resource
 * @param resource The handle of the resource to get the value
 * @param dest A buffer where to copy the data
 */
void resource_value(resource_t resource, char *dest);

/**
 * @brief Set the new value of a given resource
 * @param resource The handle of the resource to update
 * @param new_val The buffer containing the new data
 * @param len The length of the buffer. Must be < RESOURCE_DBUF_SIZE
 */
void resource_value_set(resource_t resource, char *new_val, int len);

#endif