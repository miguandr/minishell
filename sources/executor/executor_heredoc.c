/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:27:45 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/31 13:24:37 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Generates a unique filename for the heredoc temporary file.
 *
 * This function generates a unique filename for a heredoc by incrementing a
 * static counter and concatenating it with a predefined path. It returns
 * the generated filename.
 */
char	*generate_name(void)
{
	static int	i;
	char		*file_num;
	char		*file_name;

	i = 0;
	file_num = ft_itoa(i++);
	file_name = ft_strjoin("/tmp/heredoc_", file_num);
	free(file_num);
	return (file_name);
}

/**
 * Expands variables in a heredoc input string.
 * @data: Pointer to the minishell structure containing environment
 * variables.
 * @str: The heredoc input string with potential variables.
 * @result: Buffer where the expanded result is stored.
 * @result_len: Current length of the result buffer.
 *
 * This function searches for variables in the heredoc input string starting
 * with a '$' and replaces them with their corresponding values from the
 * environment. It updates the result buffer with the expanded value and
 * returns the length of the variable name plusgenerate_name one for the '$'
 * character.
 */
static int	exp_var_hd(t_mshell *data, char *str, char *result, int result_len)
{
	char	*var_name;
	char	*var_value;
	int		var_name_len;
	int		var_value_len;

	var_value_len = 0;
	var_name = get_variable_name(str + 1, data);
	var_name_len = ft_strlen(var_name);
	var_value = get_variable_value(data, var_name);
	if (var_value)
	{
		var_value_len = ft_strlen(var_value);
		ft_strcpy(result + result_len, var_value);
		result_len += var_value_len;
		free(var_value);
	}
	free(var_name);
	return (var_name_len + 1);
}

/**
 * Expands the heredoc input by processing variables within the string.
 * @data: Pointer to the minishell structure containing environment variables.
 * @str: The heredoc input string to be expanded.
 *
 * This function iterates through the input string, expanding variables and
 * building the final result string. It allocates memory for the result and
 * returns it after expanding any variables found in the input.
 */
static char	*expand_heredoc(t_mshell *data, char *str)
{
	char	*result;
	int		result_len;
	int		i;
	int		str_len;

	i = 0;
	result_len = 0;
	str_len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	if (!result)
		handle_error(data, 0);
	while (i < str_len)
	{
		if (str[i] == '$')
		{
			i += exp_var_hd(data, str + i, result, result_len);
			result_len = ft_strlen(result);
		}
		else
			result[result_len++] = str[i++];
	}
	result[result_len] = '\0';
	return (result);
}

/**
 * Handles the input for a heredoc, expanding variables and writing the
 * results to a file.
 * @data: Pointer to the minishell structure containing environment variables.
 * @file: File descriptor for the heredoc file.
 * @delimiter: Delimiter string indicating the end of the heredoc input.
 *
 * This function reads lines of input from the user, expands any variables,
 * and writes the expanded lines to the specified file. It continues until
 * the delimiter is encountered.
 */
static int	ft_handle_hd_input(t_mshell *data, int file, char *delimiter)
{
	char	*expanded_line;
	char	*input_line;

	while (1)
	{
		input_line = readline("> ");
		if (!input_line)
			return (EXIT_FAILURE);
		if (!ft_strncmp(input_line, delimiter, ft_strlen(delimiter))
			&& input_line[ft_strlen(delimiter)] == '\0')
		{
			free(input_line);
			break ;
		}
		else
		{
			expanded_line = expand_heredoc(data, input_line);
			if (expanded_line)
				ft_putendl_fd(expanded_line, file);
			free(expanded_line);
		}
		free(input_line);
	}
	return (EXIT_SUCCESS);
}

/**
 * Manages the heredoc process by setting up the file and calling the input
 * handler function.
 * @commands: Pointer to the parser structure containing heredoc details.
 * @minishell: Pointer to the minishell structure containing environment
 * variables.
 *
 * This function opens a file for the heredoc, handles errors, and then
 * delegates the input handling to another function. It ensures the file
 * is properly closed after processing.
 */
int	ft_heredoc(t_parser *commands, t_mshell *minishell)
{
	char	*delimiter;
	int		file;

	delimiter = commands->redirections->str;
	file = open(commands->hd_file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (handle_error(minishell, 8));
	if (ft_handle_hd_input(minishell, file, delimiter) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	close(file);
	return (EXIT_SUCCESS);
}
