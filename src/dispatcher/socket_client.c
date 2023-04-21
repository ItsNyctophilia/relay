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
#include <poll.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>

mtx_t lock;

bool message_ready = false;
char buffer[BUFSIZ];



static void cleanup_threads(struct thread_pool *trds, thrd_t input, thrd_t conn) {
        for (size_t n = 0; n < trds->thrd_sz; ++n) {
                if (trds->threads[n]) {

                        thrd_join(trds->threads[n], NULL);
                }
        }
        if (trds->threads) {
                free(trds->threads);
        }
        if (trds->thread_fds) {
                free(trds->thread_fds);
        }
        if (input){
                thrd_join(input, NULL);
        }
        if (conn){
                thrd_join(conn, NULL);
        }
        //free(buffer);
}

static struct client_data *create_client(int sd, int *rtn)
{
        struct pollfd input = { .fd = sd, .events = POLLIN };
        int result = poll(&input, 1,  500);
        if (result == 0) {
                *rtn = NTS;
                return NULL;
        }
	struct client_data *client = calloc(1, sizeof(*client));
	if (client == NULL) {
		perror("Failed to allocate memory for client");
		*rtn = MEMORY_ALLOCATION_ERROR;
        return NULL;
	}
	client->client_sz = sizeof(client->client_strg);


    client->client_socket =
                    accept(sd, (struct sockaddr *)&client->client_strg,
                           &client->client_sz);

    if (client->client_socket == -1) {
        perror("Failed to accept client");
        free(client);
        return NULL;
    }
    client->ptr.sd = client->client_socket;
    *rtn = CLIENT_SUCCESS;
	return client;
}

static int setup_threads(struct thread_pool *trds)
{
	if (!trds) {
		perror("No thread pool");
		return NULL_PARAMETER;
	}
	trds->thrd_sz = 0;
	// Seems like reasonable number of starting threads
	trds->thrd_cap = 8;
	trds->threads = malloc(trds->thrd_cap * sizeof(*trds->threads));
	if (!trds->threads) {
		perror("Could not allocate threads");
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
	//char buffer[15] = "MESSAGE";
	while (break_loop) {

	}
    strcpy(buffer, "\07");
    write(client.sd, buffer, BUFSIZ - 1);

	close(client.sd);
	return 0;
}

static int input_thread_func(void *arg)
{
        union thread_ptr trd = {.ptr = arg };
        struct thread_pool *trds = trd.tp;
        struct pollfd input = { .fd = 0, .events = POLLIN };
        while (break_loop) {
                int result = poll(&input, 1,  500);
                if (input.revents & POLLIN) {
                        fgets(buffer, BUFSIZ - 1, stdin);
                        for (size_t n = 0; n < trds->thrd_sz; ++n) {
                                ssize_t amount = write(trds->thread_fds[n], buffer, strlen(buffer));
                                if (amount < 0 && errno != EAGAIN) {
                                        perror("Unable to read from client");
                                        close(trds->thread_fds[n]);
                                        return CLIENT_WRITE_ERROR;
                                }
                        }
                }
        }
        return SUCCESS;
}

static int connection_thread_func(void *arg)
{
        union thread_ptr trd = {.ptr = arg };
        struct thread_pool *trds = trd.tp;
        while (break_loop) {
                int rtn;
                struct client_data *c_data = create_client(trd.tp->sd, &rtn);
                if (rtn == MEMORY_ALLOCATION_ERROR) {
                        perror("Unable to create client!!");
                        return CLIENT_CREATE_FAIL;
                } else if (rtn == NTS){
                        continue;
                }

                trds->thread_fds[trds->thrd_sz] = c_data->client_socket;
                int err =
                        thrd_create(&trds->threads[trds->thrd_sz++], handle_client,
                        c_data->ptr.ptr);

                if (err != SUCCESS) {
                        perror("Unable to create thread");
                        return THREAD_CREATE_ERROR;
                }

                if (trds->thrd_sz == trds->thrd_cap) {
                        thrd_t *tmp =
                        realloc(trds->threads,
                        2 * trds->thrd_cap * sizeof(*tmp));
                        int *tmp2 =
                        realloc(trds->thread_fds,
                            2 * trds->thrd_cap * sizeof(*tmp2));
                        if (!tmp || !tmp2) {
                                perror("Cannot create more threads");
                                break;
                        }
                        trds->threads = tmp;
                        trds->thread_fds = tmp2;
                        trds->thrd_cap *= 2;
		        }
        }
        return SUCCESS;
}

int start_client_loop(int sd)
{

        struct thread_pool trds = { 0 };
        trds.sd = sd;
        trds.thread_fds = malloc(trds.thrd_cap * sizeof(*trds.thread_fds));
        int rtn = setup_threads(&trds);
        int err = SUCCESS;
        if (rtn != SUCCESS) {
                perror("Unable to setup threads");
                err = rtn;
        }

        thrd_t input_thread;
        if (err == SUCCESS) {

                union thread_ptr input_ptr = {.tp = &trds};
                err = thrd_create(&input_thread, input_thread_func, input_ptr.ptr);
                if (err != SUCCESS) {
                        perror("Unable to create thread");
                        err = THREAD_CREATE_ERROR;
                }
        }

        thrd_t connection_thread;
        if (err == SUCCESS) {
                union thread_ptr conn_ptr = {.tp = &trds};
                err = thrd_create(&connection_thread, connection_thread_func, conn_ptr.ptr);
                if (err != SUCCESS) {
                        perror("Unable to create thread");
                        err = THREAD_CREATE_ERROR;
                }
        }
        while (break_loop) {

        }
        cleanup_threads(&trds, input_thread, connection_thread);

	    return err;
}
