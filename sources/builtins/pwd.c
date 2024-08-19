/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 01:12:17 by marvin            #+#    #+#             */
/*   Updated: 2024/06/15 01:12:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h" //modifica el nombre

int	mini_pwd(t_mshell *minishell, t_parser *commands)
{
	(void)commands;
	
	ft_putendl_fd(minishell->pwd, 1);
	return (EXIT_SUCCESS);
}
