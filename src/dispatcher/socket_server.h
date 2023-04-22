/**
 * @file socket_server.h
 * @brief 
 * @version 0.1
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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
	UNLINK_FAIL,
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

/**
 * @brief Create a server object
 *
 * @param err
 * @return struct socket_server*
 */
struct socket_server *create_server(int *err);

/**
 * @brief Destroy the server object
 * Close the socket, free memory and unlink the socket file
 * @param server
 * @return int
 */
int destroy_server(struct socket_server *server);

#endif				//RELAY_SOCKET_SERVER_H
