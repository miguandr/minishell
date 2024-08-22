/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:57:58 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:57:58 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	mini_pwd(t_mshell *minishell, t_parser *commands)
{
	(void)commands;

	ft_putendl_fd(minishell->pwd, 1);
	return (EXIT_SUCCESS);
}
