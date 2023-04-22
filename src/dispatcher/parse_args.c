#include "parse_args.h"
#include "../common.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static struct option long_opts[] = {
	{"limit", required_argument, NULL, 'l'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0}
};

static void print_help(void)
{
	printf("USAGE:\n");
	printf("./dispatcher [OPTIONS]\n");
	printf
	    ("\t--limit/-l\tSpecifies the maximum number of listener instances\n");
	printf("\t--help/-h\tShow usage help\n");
}

int parse_command_line(int argc, char **argv[], struct app *app)
{
	int opt;
	if (!app) {
		return FAILURE;
	}
	while ((opt = getopt_long(argc, *argv, "l:h", long_opts, NULL)) != -1) {
		switch (opt) {
		case 'l':
			if (optarg) {
				char *endptr;
				app->limit = strtol(optarg, &endptr, 10);
				if (*endptr != '\0') {
					fprintf(stderr,
						"Invalid limit value '%s', numerical required!\n",
						endptr);
					print_help();
					return FAILURE;
				}
				if (app->limit < 1) {
					fprintf(stderr,
						"Limit must be greater than 1!\n");
					print_help();
					return FAILURE;
				}
                app->limit_set = true;
			} else {
				fprintf(stderr, "Limit value is missing!\n");
				return FAILURE;
			}
			break;
		case 'h':
			print_help();
			return FAILURE;
		case '?':
			fprintf(stderr, "Improper usage!\n");
			print_help();
		}
	}
	return SUCCESS;
}
