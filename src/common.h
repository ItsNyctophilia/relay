//
// Created by user on 4/20/23.
//

#ifndef RELAY_D_COMMON_H
#define RELAY_D_COMMON_H
#include <stdbool.h>
#include <stdio.h>

enum {
    SUCCESS,
    FAILURE
};

struct app {
    bool no_buffer;
    long limit;
    int clients;
};

struct msg {
    char *data;
    size_t size;
    int code;
};

#endif //RELAY_D_COMMON_H
