//
// Created by user on 4/20/23.
//

#ifndef RELAY_SOCKET_CLIENT_H
#define RELAY_SOCKET_CLIENT_H
#include "socket_server.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <threads.h>
#include <stdbool.h>

extern bool break_loop;
extern bool message_ready;
extern char *buffer;

enum {
    CLIENT_SUCCESS,
    CLIENT_CREATE_FAIL,
    CLIENT_CONNECT_FAIL,
    CLIENT_READ_FAIL
};


struct client_data {
	int client_socket;
	struct sockaddr_storage client_strg;
	socklen_t client_sz;
	union socket_ptr ptr;
};

struct thread_pool {
    int sd;
	thrd_t *threads;
	size_t thrd_sz;
	size_t thrd_cap;
    int *thread_fds;
};

union thread_ptr {
    void *ptr;
    struct thread_pool *tp;
};

int start_client_loop(int sd);

#endif				//RELAY_SOCKET_CLIENT_H
