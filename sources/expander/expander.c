/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:58:50 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/12 15:56:51 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"

static int	handle_sing_quote(t_mshell *data, char *str, int *i, char *result)
{
	int	result_len;

	result_len = 0;
	result[result_len++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'')
	{
		if (str[*i] == '$')
			result_len += append_expanded(data, str, i, result + result_len);
		else
			result[result_len++] = str[(*i)++];
	}
	if (str[*i] == '\'')
		result[result_len++] = str[(*i)++];
	return (result_len);
}

static int	handle_double_quote(t_mshell *data, char *str, int *i, char *result)
{
	int	result_len;
	int	len;

	result_len = 0;
	len = ft_strlen(str);
	while (*i < len && str[*i] != '\"')
	{
		if (str[*i] == '$')
			result_len += append_expanded(data, str, i, result + result_len);
		else if (str[*i] == '\'')
			result_len += handle_sing_quote(data, str, i, result + result_len);
		else
			result[result_len++] = str[(*i)++];
	}
	return (result_len);
}

char	*expand_double_quote(t_mshell *data, char *str)
{
	char	*result;
	int		result_len;
	int		i;
	int		len;

	i = 0;
	result_len = 0;
	len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	if (!result)
		handle_error(data, 0);
	while (i < len)
	{
		if (str[i] == '\"')
		{
			i++;
			result_len += handle_double_quote(data, str, &i, result + result_len);
			if (str[i] == '\"')
				i++;
		}
		else
			result[result_len++] = str[i++];
	}
	result[result_len] = '\0';
	return (result);
}

char	*expand_str(t_mshell *data, char *str, bool *flag)
{
	if (ft_strchr(str, '\"') != NULL && str[0] != '\'')
		return (expand_double_quote(data, str));
	else if (ft_strchr(str, '$') != NULL)
		return (expand_variable_helper(data, str, flag));
	else if (ft_strchr(str, '\'') != NULL)
		return (remove_single_quote(str, data));
	return (str);
}

void	expander(t_mshell *data, char **str, bool *flag)
{
	char	*expanded_str;
	int		i;

	i = 0;
	while (str[i] != NULL)
	{
		expanded_str = expand_str(data, str[i], flag);
		if (expanded_str != str[i])
		{
			free(str[i]);
			str[i] = expanded_str;
		}
		i++;
	}
}
