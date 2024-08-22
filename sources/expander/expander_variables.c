/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variables.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:39:40 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:14:18 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Extracts the name of a variable from a string.
 * @str: The input string containing the variable name.
 * @data: Pointer to the t_mshell structure containing shell state.
 *
 * This function extracts the variable name from the input string, ensuring
 * it consists of alphanumeric characters or underscores.
 * Returns the extracted variable name.
 */
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

/**
 * Retrieves the value of a shell variable from the environment.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @var_name: The name of the variable to retrieve.
 *
 * This function searches the shell's environment for the specified
 * variable and returns its value.
 * If the variable is not found, returns NULL.
 */
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

/**
 * Expands a shell variable and appends its value to the result string.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the variable reference.
 * @i: Pointer to the current index in the string, updated during processing.
 * @result: The output buffer where the expanded value is stored.
 *
 * This function expands the specified variable by retrieving its value
 * and copying it to the result buffer.
 * Returns the length of the expanded value.
 */
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
