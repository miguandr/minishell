/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:35:19 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:35:19 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//It takes an argument str and returns a function pointer that accepts
//two arguments: t_mshell *minishell and t_parser *commands
//Checks if the input string matches a built-in command name.
//If matched, verifies that the lengths of the strings are equal to avoid
//partial matches like "echoo", and returns the function pointer for the
//matched built-in command.
//If no match is found, returns NULL.
int	(*builtins_handler(char *str))(t_mshell *minishell, t_parser *commands)
{
	static void	*builtins_array [7][2] = {
	{"echo", mini_echo},
	{"cd", mini_cd},
	{"pwd", mini_pwd},
	{"export", mini_export},
	{"unset", mini_unset},
	{"env", mini_env},
	{"exit", mini_exit},
	};
	int			i;

	i = 0;
	while (i < 7)
	{
		if (str && !ft_strncmp(builtins_array[i][0], str, ft_strlen(str)))
			return (builtins_array[i][1]);
		else
			i++;
	}
	return (NULL);
}

//Searches the $ in the string.
//Extracts the variable name up to the $.
//Copies the remainder of the string after the $.
//var_value --> searches in the env the value of the variable to expand.
//Joins the variable name and value into a single string.
//Returns the expanded string if successful, otherwise returns NULL.
char	*expand_builtin(t_mshell *data, const char *str)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	*expanded_str;
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			var_name = ft_substr(str, 0, i);
			i++;
			temp = strdup(str + i);
		}
		else
			i++;
	}
	var_value = get_variable_value(data, temp);
	expanded_str = ft_strjoin(var_name, var_value);
	free(var_name);
	free(var_value);
	if (expanded_str)
		return (expanded_str);
	return (NULL);
}

//Checks if the first string in the array contains a $.
//If found, attempts to expand the variable.
//If it is successful, replaces the original string with the expanded variable.
char	**expander_builtins(t_mshell *data, char **str)
{
	char	*expanded_str;

	if (str[0])
	{
		if (ft_strchr(str[0], '$') != NULL)
		{
			expanded_str = expand_builtin(data, str[0]);
			if (expanded_str)
			{
				free(str[0]);
				str[0] = expanded_str;
			}
		}
	}
	return (str);
}
