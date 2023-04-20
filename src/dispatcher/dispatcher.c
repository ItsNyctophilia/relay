//
// Created by user on 4/20/23.
//

#include "../common.h"
#include "parse_args.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct app app = {0};
    if (parse_command_line(argc, &argv, &app) == FAILURE) {
        return FAILURE;
    }
}