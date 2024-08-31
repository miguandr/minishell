/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:58:50 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/31 12:54:27 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the expansion of the shell's exit code within a string.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the exit code reference.
 * @i: Pointer to the current index in the string, updated during processing.
 * @result: The output buffer where the expanded exit code is stored.
 *
 * This function converts the shell's exit code to a string and appends it to
 * the result buffer.
 * It skips past the exit code reference in the input string.
 * Returns the length of the expanded exit code string.
 */
static int	handle_exit_code(t_mshell *data, char *str, int *i, char *result)
{
	char	*exit_code;
	int		result_len;

	result_len = 0;
	exit_code = ft_itoa(data->exit_code);
	while (exit_code[result_len])
	{
		result[result_len] = exit_code[result_len];
		result_len++;
	}
	*i += 2;
	while (str[*i] && ft_isalnum(str[*i]) && ft_isalpha(str[*i]))
		result[result_len++] = str[(*i)++];
	return (result_len);
}

/**
 * Handles the processing of a double-quoted string segment,
 * expanding variables within it.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing the segment.
 * @i: Pointer to the current index in the string, updated during processing.
 * @result: The output buffer where the processed segment is stored.
 *
 * This function processes characters within double quotes, expanding variables
 * and handling embedded single quotes.
 * It appends the processed characters to the result buffer.
 * Returns the length of the processed segment.
 */
static int	handle_double_quote(t_mshell *data, char *str, int *i, char *result)
{
	int	result_len;
	int	len;

	result_len = 0;
	len = ft_strlen(str);
	while (*i < len && str[*i] != '\"')
	{
		if (str[*i] == '$' && str[*i + 1] == '?')
			result_len += handle_exit_code(data, str, i, result + result_len);
		if (str[*i] == '$' && (ft_isalnum(str[*i + 1])))
			result_len += append_expanded(data, str, i, result + result_len);
		else if (str[*i] == '\'')
			result_len += handle_sing_quote(data, str, i, result + result_len);
		else
			result[result_len++] = str[(*i)++];
	}
	return (result_len);
}

/**
 * Expands variables and quotes within a double-quoted string.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string containing double-quoted segments.
 *
 * This function iterates through the input string, processing and expanding
 * any double-quoted segments.
 * It handles variable expansion and returns the fully expanded string.
 * Returns the expanded string.
 */
char	*expand_double_quote(t_mshell *data, char *str)
{
	char	*result;
	int		res_len;
	int		i;
	int		len;

	i = 0;
	res_len = 0;
	len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	if (!result)
		handle_error(data, 0);
	while (i < len)
	{
		if (str[i] == '\"')
		{
			i++;
			res_len += handle_double_quote(data, str, &i, result + res_len);
			if (str[i] == '\"')
				i++;
		}
		else
			result[res_len++] = str[i++];
	}
	result[res_len] = '\0';
	return (result);
}

/**
 * Expands variables and removes quotes within a string.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: The input string to be expanded.
 * @flag: Pointer to a boolean flag used to indicate if a variable was expanded.
 *
 * This function checks for the presence of quotes or variables in the string
 * and processes them accordingly.
 * Returns the expanded string.
 */
char	*expand_str(t_mshell *data, char *str, bool *flag)
{
	if (ft_strchr(str, '\"') != NULL && str[0] != '\'')
		return (expand_double_quote(data, str));
	else if (ft_strchr(str, '$') != NULL && ft_strchr(str, '$')[1] != '\0')
		return (expand_variable_helper(data, str, flag));
	else if (ft_strchr(str, '\'') != NULL)
		return (remove_single_quote(str, data));
	return (str);
}

/**
 * Expands all strings in an array by processing quotes and variables.
 * @data: Pointer to the t_mshell structure containing shell state.
 * @str: Array of strings to be expanded.
 * @flag: Pointer to a boolean flag used to indicate if a variable was expanded.
 *
 * This function iterates through the array of strings, expanding each one by
 * processing embedded quotes and variables.
 * The original strings are freed and replaced with their expanded versions.
 */
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
