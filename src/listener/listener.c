//
// Created by user on 4/20/23.
//
#include "../common.h"
#include "../signal_hdlr.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


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
}