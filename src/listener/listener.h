//
// Created by user on 4/21/23.
//

#ifndef RELAY_LISTENER_H
#define RELAY_LISTENER_H
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

enum {
    LISTENER_SUCCESS,
    LISTENER_CREATE_FAIL,
    LISTENER_CONNECT_FAIL,
    LISTENER_READ_FAIL
};

struct listener {
    int sd;
    struct sockaddr_un endpoint;
    int err;
};

#endif //RELAY_LISTENER_H
