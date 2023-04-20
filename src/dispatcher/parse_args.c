#include "parse_args.h"
#include "d_common.h"
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static struct option long_opts[] = {
        {"buffer_false", no_argument, NULL, 'b'},
        {"limit", required_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
};

static void print_help(void)
{
    printf("USAGE:\n");
    printf("./dispatcher [OPTIONS]\n");
    printf
            ("\t--buffer_false/-b\tAdd a -b flag to dispatcher which allows it to read from stdin "
             "without buffering by line\n");
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
    while ((opt = getopt_long(argc, *argv, "bl:h", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'b':
                app->no_buffer = true;
                break;
            case 'l':
                if (optarg) {
                    char *endptr;
                    app->limit = strtol(optarg, &endptr, 10);
                    if (*endptr != '\0') {
                        fprintf(stderr, "Invalid limit value (%s)\n", endptr);
                        return FAILURE;
                    }
                    if (app->limit < 1) {
                        fprintf(stderr, "Limit must be greater than 1\n");
                        return FAILURE;
                    }
                }
                break;
            case 'h':
                print_help();
                return FAILURE;
            case '?':
                fprintf(stderr, "Something went wrong\n");
        }
    }
    return SUCCESS;
}

