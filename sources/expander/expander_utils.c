/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:26:59 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:20:43 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Removes single quotes from a string.
 * @str: The input string from which to remove single quotes.
 * @data: Pointer to the t_mshell structure containing shell state.
 *
 * This function iterates through the input string, copying characters
 * to a new buffer while skipping single quotes.
 * Returns the new string without single quotes.
 */
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

/**
 * Handles the processing of a single-quoted string segment,
 * expanding variables within it.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the segment.
 * @i: Pointer to the current index in the string, updated during processing.
 * @result: The output buffer where the processed segment is stored.
 *
 * This function processes characters within single quotes, expanding
 * variables as needed.
 * It appends the processed characters to the result buffer.
 * Returns the length of the processed segment.
 */
int	handle_sing_quote(t_mshell *data, char *str, int *i, char *result)
{
	int	result_len;

	result_len = 0;
	result[result_len++] = str[(*i)++];
	while (str[*i] && str[*i] != '\'' && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1] != '\0')
			result_len += append_expanded(data, str, i, result + result_len);
		else
			result[result_len++] = str[(*i)++];
	}
	if (str[*i] == '\'')
		result[result_len++] = str[(*i)++];
	return (result_len);
}

/**
 * Adds the exit code to the result string, handling any subsequent characters.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @var_value: The string representation of the exit code.
 * @str: The input string containing the exit code reference.
 * @i: Pointer to the current index in the string, updated during processing.
 *
 * This function appends the exit code value to the result buffer and processes
 * any characters following the exit code reference.
 * Returns the updated result string.
 */
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

/**
 * Expands a shell variable within a string, handling special cases like &?.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the variable reference.
 * @index: Pointer to the current index in the string, updated during processing.
 *
 * This function checks for special cases like the $? (represented by `$?`)
 * expands the variable,
 * and returns its value. The index is updated to skip the expanded variable in
 * the input string.
 * Returns the expanded variable value or NULL if the variable does not exist.
 */
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

/**
 * Helper function for expanding variables within a string, handling special
 * cases and removing single quotes.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the variable reference.
 * @flag: Pointer to a boolean flag used to indicate if a variable was expanded.
 *
 * This function expands variables, handles special cases like single quotes,
 * and updates the flag if necessary.
 * Returns the expanded string or the original string if no expansion occurs.
 */
char	*expand_variable_helper(t_mshell *data, char *str, bool *flag)
{
	char	*expanded_str;
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
