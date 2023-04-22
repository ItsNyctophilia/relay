/**
 * @file signal_hdlr.h
 * @brief 
 * @version 0.1
 * @date 2023-04-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef RELAY_SIGNAL_HDLR_H
#define RELAY_SIGNAL_HDLR_H
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>
#include <stdbool.h>
#include "common.h"

extern bool break_loop;

/**
 * @brief Configures and handles signals sent to program
 *
 * @param int signal
 * @param siginfo_t info
 * @param void *context
 */
void sig_handler(int signal, siginfo_t * info, void *context);

/**
 * @brief Set the signals object
 *
 * @param action
 * @return int
 */
int set_signals(struct sigaction *action);

#endif				//RELAY_SIGNAL_HDLR_H
