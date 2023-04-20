//
// Created by user on 4/20/23.
//

#include "socket_server.h"
#include "../common.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


struct socket_server *create_server(int *err)
{
    struct socket_server *server = calloc(1, sizeof(*server));
    if (server == NULL) {
        perror("Failed to allocate memory for server");
        return NULL;
    }

    server->pathname = calloc(sizeof(char), PATH_LEN);
    strcpy(server->pathname, "/tmp/relay.sock");

    server->sd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server->sd < 0) {
        perror("Socket creation failed");
        *err = SOCKET_CREATE_FAIL;
        return NULL;
    }

    server->endpoint.sun_family = AF_UNIX;
    // test to see if socket file exists
    if (access(server->pathname, F_OK) != -1) {
        // file exists
        if (unlink(server->pathname) < 0) {
            perror("Failed to unlink socket file");
            *err = SOCKET_CREATE_FAIL;
            return NULL;
        }
    }

    strcpy(server->endpoint.sun_path, server->pathname);

    server->err = bind(server->sd, (struct sockaddr *)&server->endpoint, sizeof(server->endpoint));
    if (server->err < 0) {
        perror("Bind failed");
        *err = BIND_FAIL;
        return NULL;
    }

    server->err = listen(server->sd, 5);
    if (server->err < 0) {
        perror("Listen failed");
        *err = LISTEN_FAIL;
        return NULL;
    }

    return server;
}

struct socket_client *create_client(int sd)
{
    struct socket_client *client = calloc(1, sizeof(*client));
    if (client == NULL) {
        perror("Failed to allocate memory for client");
        return NULL;
    }
    client->client_sz = sizeof(client->client_strg);
    client->client_socket = accept(sd, (struct sockaddr *)&client->client_strg, &client->client_sz);
    return client;
}

int destroy_server(struct socket_server *server)
{
    if (server == NULL) {
        return NULL_PARAMETER;
    }
    if (server->sd > 0) {
        close(server->sd);
    }
    if (server->pathname != NULL) {
        free(server->pathname);
    }
    free(server);
    // delete socket file
    unlink("/tmp/relay.sock");
    return SUCCESS;
}