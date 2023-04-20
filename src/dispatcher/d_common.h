//
// Created by user on 4/20/23.
//

#ifndef RELAY_D_COMMON_H
#define RELAY_D_COMMON_H
#include <stdbool.h>

enum {
    SUCCESS,
    FAILURE
};

struct app {
    bool no_buffer;
    long limit;
    int clients;
};

#endif //RELAY_D_COMMON_H
