/**
 * @file listener.h
 * @brief 
 * @version 0.1
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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

#endif				//RELAY_LISTENER_H
