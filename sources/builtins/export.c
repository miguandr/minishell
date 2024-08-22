/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:58:05 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:05 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks for a matching environment variable and updates it if found.
 * @data: Pointer to the minishell data structure.
 * @i: Index of the current environment variable being checked.
 * @name: The name of the environment variable to check for.
 * @add_var: The new value to assign if a match is found.
 *
 * This function compares the given name with the environment variable at 
 * the specified index. If a match is found, the current variable is 
 * replaced with the new value and the old value is freed. 
 * Returns 0 if a match is found and updated, otherwise returns 1.
 */
static int	check_coincidence(t_mshell *data, int i, char *name, char *add_var)
{
	if (!ft_strncmp(data->envp[i], name, ft_strlen(name)))
	{
		free(data->envp[i]);
		data->envp[i] = add_var;
		return (0);
	}
	return (1);
}

/**
 * Verifies if a character is valid as part of a variable identifier in Bash.
 * @c: The character to validate.
 *
 * This function checks whether the given character is a valid part of a 
 * Bash variable identifier, which includes letters, digits, and underscores.
 * Returns true if the character is valid, otherwise returns false.
 */
static bool	check_valid_identifier(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_' )
		return (true);
	else
		return (false);
}

/**
 * @str: The input string containing the variable definition.
 * @var_name: ptr to a string where the variable name will be stored w/o quotes.
 * @minishell: Pointer to the minishell data structure.
 *
 * This function removes quotes from the input string. If an '=' character 
 * is found, the function extracts the variable name before the '=' and 
 * removes quotes from it, storing the result in var_name. The entire 
 * string without quotes is returned.
 * Returns the processed string without quotes.
 */
static char	*check_quotes(char *str, char **var_name, t_mshell *minishell)
{
	int		i;
	char	*temp;
	char	*substr;

	i = 0;
	temp = delete_quotes(str, minishell);
	while (str[i])
	{
		if (str[i] == '=')
		{
			substr = ft_substr(str, 0, i);
			*var_name = delete_quotes(substr, minishell);
			free(substr);
			return (temp);
		}
		else
			i++;
	}
	*var_name = delete_quotes(str, minishell);
	return (temp);
}

/**
 * This function checks for errors in the export command by validating 
 * the first character of the argument and ensuring all characters before 
 * the '=' are valid as part of a variable identifier. If an error is found, 
 * it calls handle_error2 with the appropriate error code.
 * Returns EXIT_SUCCESS if no errors are found, otherwise returns EXIT_FAILURE.
 */
static int	error_check(t_mshell *data, t_parser *commands)
{
	int	i;

	i = 0;
	if (commands->str[1])
	{
		if (ft_isdigit(commands->str[1][0]) || commands->str[1][0] == '=')
			return (handle_error2(data, 2, commands->str[1], NULL));
		while (commands->str[1][i] != '=')
		{
			if (!check_valid_identifier(commands->str[1][i]))
			{
				if (commands->str[1][i] == '!')
					return (handle_error2(data, 3, commands->str[1] + i, NULL));
				else
					return (handle_error2(data, 4, commands->str[1], NULL));
			}
			i++;
			if (!commands->str[1][i])
				break ;
		}
	}
	return (EXIT_SUCCESS);
}

/**
 * Implements the export command, which sets or updates environment variables.
 *
 * It first checks for errors in the variable name or definition. 
 * If no errors are found, it removes quotes from the variable name and value,
 * checks if the variable already exists, and updates it if so. 
 * If the variable does not exist, it creates a new environment array 
 * with the new variable.
 * Returns EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
int	mini_export(t_mshell *minishell, t_parser *commands)
{
	int		i;
	char	**temp;
	char	*add_var;
	char	*var_name;

	i = -1;
	if (error_check(minishell, commands))
		return (EXIT_FAILURE);
	if (!commands->str[1] || commands->str[1][0] == '\0')
		mini_env(minishell, commands);
	else
	{
		add_var = check_quotes(commands->str[1], &var_name, minishell);
		while (minishell->envp[++i])
		{
			if (check_coincidence(minishell, i, var_name, add_var) == 0)
				return (EXIT_SUCCESS);
		}
		temp = new_array(minishell->envp, add_var);
		free_string_array(minishell->envp);
		minishell->envp = temp;
		free(var_name);
	}
	return (EXIT_SUCCESS);
}
