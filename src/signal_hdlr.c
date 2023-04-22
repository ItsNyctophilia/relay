#include "signal_hdlr.h"
#include "common.h"
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>

bool break_loop = true;

static int set_signal_catch(int signal, struct sigaction action)
{
	int err = sigaction(signal, &action, NULL);
	if (err < 0) {
		perror("Unable to change signal");
		return SIGNAL_CHANGE_ERROR;
	}

	return SUCCESS;
}

void sig_handler(int signal, siginfo_t * info, void *context)
{
	(void)info;
	(void)context;
	switch (signal) {
	case SIGHUP:
		// puts("SIGHUP received");
		break;
	case SIGQUIT:
		// puts("SIGQUIT received");
		break;
	case SIGTERM:
		// puts("SIGSTOP received");
		break;
	case SIGINT:
		// puts("SIGINT received");
        break_loop = false;
		break;
    case SIGPIPE:
        // puts("SIGPIPE received");
        break;
	case '?':
		fprintf(stderr, "Something went wrong\n");
	}
}

int set_signals(struct sigaction *action)
{
	enum {
		SIGNAL_SZ = 5
	};
	int signals[SIGNAL_SZ] = { SIGHUP, SIGQUIT, SIGTERM, SIGINT, SIGPIPE };
	int err;
	for (int i = 0; i < SIGNAL_SZ; ++i) {
		err = set_signal_catch(signals[i], *action);
		if (err) {
			return err;
		}
	}
	return SUCCESS;
}
