//
// Created by user on 4/20/23.
//
#include "../common.h"
#include "../signal_hdlr.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>


int main(int argc, char *argv[])
{
	int err = 0;
	struct sigaction action = { 0 };
	action.sa_sigaction = sig_handler;
	err = set_signals(&action);
	if (err < 0) {
		exit(err);
	}

	if (argc < 1) {
		printf("Usage: ./%s\n", argv[0]);
		return 1;
	}

        // Always the same for a UNIX socket
        int sd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sd < 0) {
                perror("Unable to create socket");
                return 1;
        }

        struct sockaddr_un endpoint;
        endpoint.sun_family = AF_UNIX;
        snprintf(endpoint.sun_path, sizeof(endpoint.sun_path), "/tmp/relay.sock");

         err = connect(sd, (struct sockaddr *)&endpoint, sizeof(endpoint));
        if (err < 0) {
                perror("Unable to connect to endpoint");
                return 2;
        }

// 16 is plenty for a small message
        char buffer[16];
        ssize_t amount_read = read(sd, buffer, sizeof(buffer));
        if (amount_read < 0) {
                perror("Unable to read");
                close(sd);
                return 3;
        }

        buffer[amount_read] = '\0';
        puts(buffer);

        close(sd);

}
