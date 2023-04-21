//
// Created by user on 4/20/23.
//
#include "../common.h"
#include "../signal_hdlr.h"
#include "listener.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/stat.h>


static int create_socket(struct listener *l)
{
        if (!l) {
                perror("No listener");
                return NULL_PARAMETER;
        }
        // Always the same for a UNIX socket
        l->sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (l->sd < 0) {
                perror("Unable to create socket");
                return LISTENER_CREATE_FAIL;
        }
        l->endpoint.sun_family = AF_UNIX;
        snprintf(l->endpoint.sun_path, sizeof(l->endpoint.sun_path), "/tmp/relay.sock");

        l->err = connect(l->sd, (struct sockaddr *)&l->endpoint, sizeof(l->endpoint));
        if (l->err < 0) {
                perror("Unable to connect to endpoint");
                return LISTENER_CONNECT_FAIL;
        }
        return SUCCESS;
}

static int read_loop(struct listener *l)
{
        char buffer[24];
        ssize_t amount_read = 0;
        if (!l) {
                perror("No listener");
                return NULL_PARAMETER;
        }
        while (1) {
                // Always the same for a UNIX socket
                amount_read = read(l->sd, buffer, sizeof(buffer));
                if (errno == 3) {
                        break;
                }
                if (amount_read < 0) {
                        perror("Unable to read");
                        return LISTENER_READ_FAIL;
                }
                buffer[amount_read] = '\0';
                if (strcmp(buffer, "\07") == 0) {
                        break;
                }
                if (strcmp(buffer, "") != 0) {
                        printf("%s", buffer);
                }
        }
        return SUCCESS;
}

int main(int argc, char *argv[])
{
        chmod("./listener", 0777);
	if (argc < 1) {
		printf("Usage: ./%s\n", argv[0]);
		return 1;
	}
        struct listener l = { 0 };
        l.err = create_socket(&l);
        if (l.err) {
                return l.err;
        }
        l.err = read_loop(&l);
        if (l.err) {
                return l.err;
        }

        close(l.sd);

}
