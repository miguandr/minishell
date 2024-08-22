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

//echo command prints in the standar output the string passed as argument
//followed by a new line (ft_putendl_fd)
//echo -n doesn't print a new line (ft_putstr_fd)
//bash accept multiples -n


static int no_nl(t_parser *commands, int i)
{
	while (commands->str[i] && !ft_strncmp (commands->str[i], "-n", 3))
		i++;
	while(commands->str[i])
	{
		ft_putstr_fd(commands->str[i], 1);
		i++;
		if (commands->str[i])
			write(1, " ", 1);
	}
	return(i);
}

int flag (int i, t_parser *commands)
{
	i++;
	commands->flag = false;
	if (!commands->str[i])
		write(1, "\n", 1);
	return(i);
	
}
int	mini_echo(t_mshell *minishell, t_parser *commands)
{
	int		i;
	(void) minishell;
	
	i = 1;
	if(!commands->str[1]) //solo tengo echo
		write(1, "\n", 1);	
	while (commands->str[i]) //str = array
	{
		if (!ft_strncmp (commands->str[1], "-n", 3))
			i = no_nl(commands, i);
		else
		{
			if(commands->flag == true) //si no logro expandir y no habian single quotes, no tiene que imprimirlo
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
