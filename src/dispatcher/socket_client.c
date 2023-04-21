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

bool message_ready = false;
char *buffer;

static void cleanup_threads(struct thread_pool *trds)
{
        for (size_t n = 0; n < trds->thrd_sz; ++n) {
                thrd_join(trds->threads[n], NULL);
        }
        free(trds->threads);
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
    client->ptr.sd = client->client_socket;
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
	//char buffer[15] = "MESSAGE";
	while (break_loop) {
		// Add space for NUL byte
//        if (message_ready){
//                ssize_t amount = write(client.sd, buffer, BUFSIZ - 1);
//                if (amount < 0 && errno != EAGAIN) {
//                        perror("Unable to read from client");
//                        close(client.sd);
//                        return CLIENT_WRITE_ERROR;
//                }
//        }

//		if (amount > 0) {
//			int result = mtx_trylock(&lock);
//			if (result == thrd_success) {
//				buffer[amount] = '\0';
//				break;
//			}
//		}
	}

	close(client.sd);
	return 0;
}

static int input_thread_func(void *arg)
{
        union thread_ptr trd = {.ptr = arg };
        struct thread_pool *trds = trd.tp;
        size_t sz = BUFSIZ;
        while (break_loop) {
                ssize_t amount = getline(&buffer, &sz, stdin);
                if (amount == -1) {
                        break;
                }

                for (size_t n = 0; n < trds->thrd_sz; ++n) {
                        amount = write(trds->thread_fds[n], buffer, strlen(buffer));
                        if (amount < 0 && errno != EAGAIN) {
                                perror("Unable to read from client");
                                close(trds->thread_fds[n]);
                                return CLIENT_WRITE_ERROR;
                        }
                }
        }
        // Send teardown message on EOF to listeners
        for (size_t n = 0; n < trds->thrd_sz; ++n) {
                buffer[0] = '\07';
                buffer[1] = '\0';
                write(trds->thread_fds[n], buffer, 2);
        }
        return SUCCESS;
}

static int connection_thread_func(void *arg)
{
        union thread_ptr trd = {.ptr = arg };
        struct thread_pool *trds = trd.tp;
        while (break_loop) {
                struct client_data *c_data = create_client(trd.tp->sd);
                if (!c_data) {
                        perror("Unable to create client");
                        return CLIENT_CREATE_FAIL;
                }
                trds->thread_fds[trds->thrd_sz] = c_data->client_socket;
                int err =
                        thrd_create(&trds->threads[trds->thrd_sz++], handle_client,
                        c_data->ptr.ptr);

                if (err != SUCCESS) {
                        perror("Unable to create thread");
                        cleanup_threads(trds);
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
                                cleanup_threads(trds);
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
        // create the input thread
        struct thread_pool trds = { 0 };
        trds.sd = sd;
        trds.thread_fds = malloc(trds.thrd_cap * sizeof(*trds.thread_fds));
	int rtn = setup_threads(&trds);
	if (rtn != SUCCESS) {
		perror("Unable to setup threads");
            cleanup_threads(&trds);
		return rtn;
	}

        thrd_t input_thread;
        union thread_ptr input_ptr = {.tp = &trds};
        int err = thrd_create(&input_thread, input_thread_func, input_ptr.ptr);
        if (err != SUCCESS) {
                perror("Unable to create thread");
                cleanup_threads(&trds);
                return THREAD_CREATE_ERROR;
        }


        thrd_t connection_thread;
        union thread_ptr conn_ptr = {.tp = &trds};
        err = thrd_create(&connection_thread, connection_thread_func, conn_ptr.ptr);
        if (err != SUCCESS) {
                perror("Unable to create thread");
                cleanup_threads(&trds);
                return THREAD_CREATE_ERROR;
        }



        cleanup_threads(&trds);
        thrd_join(input_thread, NULL);
        //thrd_join(connection_thread, NULL);
	return SUCCESS;
}
