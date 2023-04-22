/**
 * @file common.h
 * @brief 
 * @version 0.1
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef RELAY_D_COMMON_H
#define RELAY_D_COMMON_H
#include <stdbool.h>
#include <stdio.h>
#include <threads.h>
#include <stdbool.h>

enum {
	SUCCESS,
	FAILURE,
	SIGNAL_SET_ERROR,
	SIGNAL_CHANGE_ERROR,
	NULL_PARAMETER,
	MEMORY_ALLOCATION_ERROR,
	THREAD_CREATE_ERROR,
	NON_BLOCKING_ERROR,
	CLIENT_WRITE_ERROR,
	NTS,
	CONN_LIMIT
};

struct app {
	long limit;
	bool limit_set;
	int clients;
};

struct msg {
	char *data;
	size_t size;
	int code;
};

extern struct app app;

#endif				//RELAY_D_COMMON_H
