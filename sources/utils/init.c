/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 21:10:52 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:37:37 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_data(t_mshell *data)
{
	data->commands = NULL;
	data->lexer_list = NULL;
	data->reset = false;
	data->pid = NULL;
	data->pipes = 0;
	data->in_cmd = 0;
	handle_envp(data);
	init_signals();
	return (1);
}

int	reset_data(t_mshell *data)
{
	ft_commands_clear(&data->commands);
	free(data->args);
	if (data->pid)
		free(data->pid);
	ft_free_array(data->paths);

	init_data(data);
	data->reset = true;
	minishell(data);
	return (1);
}
