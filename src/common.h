//
// Created by user on 4/20/23.
//

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
    NTS
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

#endif				//RELAY_D_COMMON_H
