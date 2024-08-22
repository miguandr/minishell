/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:39:40 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:45:22 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_variable_name(const char *str, t_mshell *data)
{
	char	*var_name;
	int		len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	var_name = ft_calloc(len + 1, sizeof(char));
	if (!var_name)
		handle_error(data, 0);
	ft_strncpy(var_name, str, len);
	return (var_name);
}

char	*get_variable_value(t_mshell *data, char *var_name)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var_name);
	while (data->envp[i] != NULL)
	{
		if (ft_strncmp(data->envp[i], var_name, var_len) == 0
			&& data->envp[i][var_len] == '=')
			return (ft_strdup(data->envp[i] + var_len + 1));
		i++;
	}
	return (NULL);
}
