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

struct client_data {
	int client_socket;
	struct sockaddr_storage client_strg;
	socklen_t client_sz;
	union socket_ptr ptr;
};

struct thread_pool {
	thrd_t *threads;
	size_t thrd_sz;
	size_t thrd_cap;
};

int start_client_loop(int sd);

#endif				//RELAY_SOCKET_CLIENT_H
