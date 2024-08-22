/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:58:15 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:15 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//no more arguments than 'env' are accepted
int	mini_env(t_mshell *minishell, t_parser *commands)
{
	int	i;

	if (commands->str[1])
	{
		ft_putstr_fd("env: ‘", STDERR_FILENO);
		ft_putstr_fd(commands->str[1], STDERR_FILENO);
		ft_putendl_fd("’: No such file or directory", STDERR_FILENO);
		reset_data(minishell);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (minishell->envp[i])
	{
		ft_putendl_fd(minishell->envp[i], 1);
		i++;
	}
	return (EXIT_SUCCESS);
}
