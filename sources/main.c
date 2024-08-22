/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:31:22 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:37:11 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	minishell(t_mshell *data)
{
	lexer(data);
	parser(data);
	executor(data);
	reset_data(data);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_mshell	data;

	if (ac != 1 || av[1])
	{
		printf("Minishell does not accept arguments\n");
		exit(0);
	}
	data.envp = dup_str(envp);
	get_pwd(&data);
	init_data(&data);
	minishell(&data);
	return (0);
}
