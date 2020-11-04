#ifndef UTIL_H
#define UTIL_H

#define NOERRNO 0
#define ERRNO 1

#define CHECK(op)                                                              \
    do {                                                                       \
        if (op == -1)                                                          \
            log_error(1, #op);                                                 \
    } while (0)

void log_error(int syserr, const char *msg, ...);
void log_info(const char *msg, ...);

#endif