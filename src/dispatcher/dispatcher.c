//
// Created by user on 4/20/23.
//

#include "../common.h"
#include "parse_args.h"
#include "../signal_hdlr.h"
#include "socket_client.h"
#include "socket_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

struct app app = { 0 };

int main(int argc, char *argv[])
{
	int err = 0;
	struct sigaction action = { 0 };
	action.sa_sigaction = sig_handler;
	err = set_signals(&action);
	if (err < 0) {
		exit(err);
	}

	if (parse_command_line(argc, &argv, &app) == FAILURE) {
		return FAILURE;
	}

	int err_rtn = 0;
	struct socket_server *server = create_server(&err_rtn);
	if (err_rtn) {
		destroy_server(server);
		return err;
	}

	err = start_client_loop(server->sd);

	err = destroy_server(server);
	if (err < 0) {
		return err;
	}
	return SUCCESS;
}
