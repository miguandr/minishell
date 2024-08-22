/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:49:02 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:49:02 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_array(char **array, int i)
{
	while (array[i])
	{
		ft_putstr_fd(array[i], STDERR_FILENO);
		i++;
		if (array[i])
			write(2, " ", 1);
	}
}

char	**new_array(char **array, char *str)
{
	char	**new_array;
	int		i;

	i = 0;
	while (array[i])
		i++;
	new_array = ft_calloc((i + 2), sizeof(char *));
	if (!new_array)
	{
		handle_error(minishell, 0);
		return (NULL);
	}
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = str;
	return (new_array);
}

char	*delete_quotes(char *str, t_mshell *minishell)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	result = ft_calloc((len + 1), sizeof(char));
	if (!result)
	{
		handle_error(minishell, 0);
		return (NULL);
	}
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '\"')
			result[j++] = str[i];
		i++;
	}
	return (result);
}
