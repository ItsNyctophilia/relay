/**
 * @file parse_args.h
 * @brief 
 * @version 0.1
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef RELAY_PARSE_ARGS_H
#define RELAY_PARSE_ARGS_H
#include "../common.h"

/**
 * Parse command line arguments
 * @param argc
 * @param argv
 * @param app
 * @return
 */
int parse_command_line(int argc, char **argv[], struct app *app);

#endif				//RELAY_PARSE_ARGS_H
