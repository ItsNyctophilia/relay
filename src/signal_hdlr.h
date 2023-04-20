//
// Created by user on 4/20/23.
//

#ifndef RELAY_SIGNAL_HDLR_H
#define RELAY_SIGNAL_HDLR_H
#include <stdint.h>
#include <signal.h>
#include <sys/types.h>

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
