#ifndef COAP_H
#define COAP_H

#include "resource.h"
#include <semaphore.h>

/**
 * @brief Fetch data at coap endpoint for resource
 * @param @struct resource res the resource to fetch data
 * @return -1 on error, 0 on success
 */
int retrieve(struct resource *res);

#endif