//
// Created by user on 4/20/23.
//
#include "socket_client.h"
#include "../common.h"
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

mtx_t lock;

static void cleanup_threads(struct thread_pool *trds)
{
        for (size_t n = 0; n < trds->thrd_sz; ++n) {
                thrd_join(trds->threads[n], NULL);
        }
}

static struct client_data *create_client(int sd)
{
	struct client_data *client = calloc(1, sizeof(*client));
	if (client == NULL) {
		perror("Failed to allocate memory for client");
		return NULL;
	}
	client->client_sz = sizeof(client->client_strg);
	client->client_socket =
	    accept(sd, (struct sockaddr *)&client->client_strg,
		   &client->client_sz);
	return client;
}

static int setup_threads(struct thread_pool *trds)
{
	if (!trds) {
		perror("No thread pool");
            cleanup_threads(trds);
		return NULL_PARAMETER;
	}
	trds->thrd_sz = 0;
	// Seems like reasonable number of starting threads
	trds->thrd_cap = 8;
	trds->threads = malloc(trds->thrd_cap * sizeof(*trds->threads));
	if (!trds->threads) {
		perror("Could not allocate threads");
        cleanup_threads(trds);
		return MEMORY_ALLOCATION_ERROR;
	}
	return SUCCESS;
}

int handle_client(void *handle)
{
	union socket_ptr client = {.ptr = handle };

	int flags = fcntl(client.sd, F_GETFL);
	int err = fcntl(client.sd, F_SETFL, flags | O_NONBLOCK);
	if (err < 0) {
		perror("Unable to set IO to nonblocking");
		close(client.sd);
		return NON_BLOCKING_ERROR;
	}
	// More than enough to detect activity
	// char buffer[15] = "MESSAGE";
	for (;;) {
		char buffer[15];
		char message[15];
        ssize_t amount = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (amount < 0 && errno != EAGAIN) {
            perror("Unable to read from user");
        }
        if (amount > 0) {
            buffer[amount] = '\0';
            int result = mtx_lock(&lock);
            if (result == thrd_success) {
                strncpy(message, buffer, sizeof(message));
                mtx_unlock(&lock);
            }
        }
		// Add space for NUL byte
		amount = write(client.sd, buffer, sizeof(buffer) - 1);
		if (amount < 0 && errno != EAGAIN) {
			perror("Unable to read from client");
			close(client.sd);
			return CLIENT_WRITE_ERROR;
		}
		if (amount > 0) {
			int result = mtx_trylock(&lock);
			if (result == thrd_success) {
				buffer[amount] = '\0';
				break;
			}
		}
	}

	close(client.sd);
	return 0;
}



int start_client_loop(int sd)
{

	struct thread_pool trds = { 0 };
	int rtn = setup_threads(&trds);
	if (rtn != SUCCESS) {
		perror("Unable to setup threads");
            cleanup_threads(&trds);
		return rtn;
	}
	while (break_loop) {
            struct client_data c_data = { 0 };
            c_data.client_sz = sizeof(c_data.client_strg);
            c_data.client_socket =
		    accept(sd, (struct sockaddr *)&c_data.client_strg,
			   &c_data.client_sz);
        if (c_data.client_socket < 0) {
                perror("Unable to accept client");
                cleanup_threads(&trds);
                return FAILURE;
        }
		c_data.ptr.sd = c_data.client_socket;

		int err =
		    thrd_create(&trds.threads[trds.thrd_sz++], handle_client,
                        c_data.ptr.ptr);
		if (err != SUCCESS) {
			perror("Unable to create thread");
            cleanup_threads(&trds);
			return THREAD_CREATE_ERROR;
		}

		if (trds.thrd_sz == trds.thrd_cap) {
			thrd_t *tmp =
			    realloc(trds.threads,
				    2 * trds.thrd_cap * sizeof(*tmp));
			if (!tmp) {
				perror("Cannot create more threads");
                cleanup_threads(&trds);
				break;
			}
			trds.threads = tmp;
			trds.thrd_cap *= 2;
		}

	}
	cleanup_threads(&trds);
	return SUCCESS;
}
