/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:26:59 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/12 15:41:15 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"

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


// char	*single_quote_helper(char *str, t_mshell *data)
// {
// 	char	*result;

// 	result = remove_single_quote(str, data);
// 	return (result);
// }

char	*expand_double_quote_helper(t_mshell *data, char *str) //creo que ya no la estas usando en ningun lado
{
	char	*result;

	result = expand_double_quote(data, str);
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
		return (var_value);
	}
	else
	{
		var_name = get_variable_name(str + 1, data);
		var_value = get_variable_value(data, var_name);
		*index += ft_strlen(var_name) + 1; //estas usando el index en algun lado??? // si, lo necesito para saber en que posicion queda el index despues de la expansion en caso de que hayan mas strings que no sean expandibles
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
	if(str[0] == '\'') //new
	 	expanded_str = (remove_single_quote(str, data)); //new
	if (expanded_str)
		return (expanded_str);
	else if (ft_strchr(str, '\'') == NULL) //new //si no hay nada que expandir y no hay single quotes
		*flag = true; //new
	return (str);
}
