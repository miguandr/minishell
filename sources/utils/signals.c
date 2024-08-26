/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 18:31:02 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/26 21:23:46 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the SIGINT (Ctrl+C) signal in a child process.
 * @sig: The signal number received (unused in this function).
 *
 * This function intercepts the SIGINT signal, typically sent when
 * the user presses Ctrl+C. It writes a newline to the standard output,
 * clears the current input line, and prepares the terminal for new input.
 */
void	handle_ctrl_c_child(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

/**
 * Handles the SIGINT (Ctrl+C) signal in the main process.
 * @sig: The signal number received (unused in this function).
 *
 * This function intercepts the SIGINT signal, writes a newline to
 * the standard output, clears the current input line, prepares the terminal
 * for new input, and refreshes the display.
 */
void	handle_ctrl_c(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * Handles the SIGQUIT (Ctrl+\) signal.
 * @sig: The signal number received (unused in this function).
 *
 * This function intercepts the SIGQUIT signal, clears the current
 * input line, and prepares the terminal for new input without
 * displaying any additional output.
 */
void	handle_ctrl_backslash(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putendl_fd("Quit (core dumped)", 2);
	rl_on_new_line();
}

/**
 * Initializes signal handling for the process.
 *
 * This function sets up custom handlers for SIGINT and SIGQUIT signals.
 * The SIGINT signal is handled by the handle_ctrl_c function, while the
 * SIGQUIT signal is ignored.
 */
void	init_signals(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
