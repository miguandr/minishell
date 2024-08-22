/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:26:59 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:45:15 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*remove_single_quote(char *str, t_mshell *data)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	result = ft_calloc(len + 1, sizeof(char));
	if (!result)
		handle_error(data, 0);
	while (i < len)
	{
		if (str[i] != '\'')
			result[j++] = str[i];
		i++;
	}
	return (result);
}

int	append_expanded(t_mshell *data, char *str, int *i, char *result)
{
	char	*var_value;
	int		var_len;

	var_len = 0;
	var_value = expand_variable(data, str + *i, i);
	if (!var_value)
		return (0);
	else if (var_value)
	{
		var_len = ft_strlen(var_value);
		ft_strcpy(result, var_value);
		free(var_value);
	}
	return (var_len);
}

static char	*add_exit_code(t_mshell *data, char *var_value, char *str, int *i)
{
	char	*result;
	int		len;

	len = ft_strlen(&str[*i]) + ft_strlen(var_value);
	result = (char *)malloc(len + 1);
	if (!result)
		handle_error(data, 0);
	ft_strcpy(result, var_value);
	ft_strlcat(result, &str[*i], len + 1);
	free(var_value);
	return (result);
}

char	*expand_variable(t_mshell *data, char *str, int *index)
{
	char	*var_name;
	char	*var_value;

	if (str[1] == '?')
	{
		var_value = ft_itoa(data->exit_code);
		*index += 2;
		if (!str[2])
			return (var_value);
		else
			return (add_exit_code(data, var_value, str, index));
	}
	else
	{
		var_name = get_variable_name(str + 1, data);
		var_value = get_variable_value(data, var_name);
		*index += ft_strlen(var_name) + 1;
		free(var_name);
		if (var_value)
			return (var_value);
		else
			return (NULL);
	}
}

char	*expand_variable_helper(t_mshell *data, char *str, bool *flag)
{
	char	*expanded_str; //esta en expand_variable
	int		expanded_i;

	expanded_i = 0;
	expanded_str = expand_variable(data, str, &expanded_i);
	if (str[0] == '\'')
		expanded_str = (remove_single_quote(str, data));
	if (expanded_str)
		return (expanded_str);
	else if (ft_strchr(str, '\'') == NULL)
		*flag = true;
	return (str);
}
