/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 16:12:01 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/12 16:40:34 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"

char	*expand_variable(t_mshell *data, char *str)
{
	char	*var_name;
	char	*var_value;

	var_name = get_variable_name(str + 1, data);
	var_value = get_variable_value(data, var_name);
	free(var_name);
	if (var_value)
		return (var_value);
	else
		return (NULL);
	}
}

char	*expand_heredoc(t_mshell *data, char *str, bool *flag)
{
	char	*expanded_str;
	int		expanded_i;

	if (ft_strchr(str, '$') != NULL)
	{
		expanded_str = expand_variable(data, str, &expanded_i);
		if (expanded_str)
			return (expanded_str);
	}
	return (str);
}
