//
// Created by user on 4/20/23.
//

#ifndef RELAY_SOCKET_SERVER_H
#define RELAY_SOCKET_SERVER_H
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>


// Length of path plus null
enum {
    PATH_LEN = 17,
};

enum {
    SVR_SUCCESS,
    BIND_FAIL,
    SOCKET_CREATE_FAIL,
    LISTEN_FAIL,
};

struct socket_server {
    int sd;
    char *pathname;
    struct sockaddr_un endpoint;
    int err;
};

union socket_ptr {
    void *ptr;
    int sd;
};

struct socket_client {
    int client_socket;
    struct sockaddr_storage client_strg;
    socklen_t client_sz;
    union socket_ptr ptr;
};

struct socket_server *create_server(int *err);

struct socket_client *create_client(int sd);

int destroy_server(struct socket_server *server);


#endif //RELAY_SOCKET_SERVER_H
