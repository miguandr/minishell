/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:57:31 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:57:31 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @commands: Pointer to the parser structure containing the command arguments.
 * @i: The current index in the command arguments array.
 *
 * It processes the "-n" flag by checking if the current argument matches the
 * flag and then printing the remaining arguments without a trailing newline. 
 * It iterates through the args and prints them with a space separating each.
 * Returns the updated index after processing the arguments.
 */
static int	no_nl(t_parser *commands, int i)
{
	while (commands->str[i] && !ft_strncmp (commands->str[i], "-n", 3))
		i++;
	while (commands->str[i])
	{
		ft_putstr_fd(commands->str[i], 1);
		i++;
		if (commands->str[i])
			write(1, " ", 1);
	}
	return (i);
}

/**
 * @i: The current index in the command arguments array.
 * @commands: Pointer to the parser structure containing the command arguments.
 *
 * This function is called when there is no "-n" flag or when other conditions 
 * require printing a newline. It disables the command flag, prints a newline 
 * if no more arguments are present, and updates the index.
 * Returns the updated index.
 */
int	flag(int i, t_parser *commands)
{
	i++;
	commands->flag = false;
	if (!commands->str[i])
		write(1, "\n", 1);
	return (i);
}

/**
 * Implements the echo command, printing arguments to the standard output.
 * @minishell: Pointer to the minishell data structure.
 * @commands: Pointer to the parser structure containing the command arguments.
 *
 * This function processes the 'echo' command. It checks for the presence of 
 * the "-n" flag, which suppresses the trailing newline. If the "-n" flag is 
 * present, it calls `no_nl` to handle the printing without a newline. If the 
 * flag is true, it prints the arguments normally and appends a nl at the end.
 * (flag will be true if there was no expansion and no single quotes.)
 * Returns EXIT_SUCCESS upon completion.
 */
int	mini_echo(t_mshell *minishell, t_parser *commands)
{
	int	i;

	(void) minishell;
	i = 1;
	if (!commands->str[1])
		write(1, "\n", 1);
	while (commands->str[i])
	{
		if (!ft_strncmp (commands->str[1], "-n", 3))
			i = no_nl(commands, i);
		else
		{
			if (commands->flag == true)
				i = flag(i, commands);
			ft_putstr_fd(commands->str[i], 1);
			i++;
			if (commands->str[i])
				write(1, " ", 1);
			else
				write(1, "\n", 1);
		}
	}
	return (EXIT_SUCCESS);
}
