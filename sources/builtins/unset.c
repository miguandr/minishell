/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:14:59 by miguandr          #+#    #+#             */
/*   Updated: 2024/07/22 23:14:59 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*rm_quote(char *str, t_mshell *minishell, int flag)
{
	int		i;
	char	*temp;
	char	*substr;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			if (flag == 1)
			{
				substr = ft_substr(str, 0, i);
				temp = delete_quotes(substr, minishell);
				free(substr);
				return (temp);
			}
			else
				return (NULL);
		}
		i++;
	}
	temp = delete_quotes(str, minishell);
	return (temp);
}

/*
 * Creates a new environment variable array, excluding variables matching a 
 * specified string.
 * @envp: The current environment variable array.
 * @str: The string to match against the environment variable names.
 *
 * This function allocates a new array, copying over all environment variables 
 * except those that match the given string. It uses ft_strdup to duplicate 
 * the strings and ft_calloc to allocate memory for the new array. 
 * If memory allocation fails, it returns NULL.
 * Returns the new array without the excluded variable, or NULL if fails.
 */
static char	**newarray(char **envp, char *str)
{
	char	**new_array;
	int		i;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	new_array = ft_calloc((i), sizeof(char *));
	if (!new_array)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], str, ft_strlen(str)))
			i++;
		if (envp[i])
			new_array[j++] = ft_strdup(envp[i++]);
	}
	return (new_array);
}

/**
 * Handles the removal of an environment variable from the shell's environment.
 * @env_var: The environment variable to be removed.
 * @unset_var: The variable being unset by the command.
 *
 * This function replaces the environment array in minishell by creating a new 
 * one without the variable to be removed. It frees the old environment array 
 * and the strings used to represent the variable to be unset.
 * Returns EXIT_SUCCESS after successfully updating the environment.
 */
static int	coincidence(t_mshell *minishell, char *env_var, char *unset_var)
{
	char	**temp;

	temp = newarray(minishell->envp, env_var);
	free_string_array(minishell->envp);
	minishell->envp = temp;
	free(env_var);
	free(unset_var);
	return (EXIT_SUCCESS);
}

/**
 * Implements the unset command, which removes environment variables.
 *
 * This function processes the 'unset' command. It removes the specified 
 * environment variable from the shell's environment. If the variable name 
 * has quotes, they are removed before comparison. The function handles 
 * errors such as invalid variable names or missing arguments.
 * Returns EXIT_SUCCESS on success, or EXIT_FAILURE on failure.
 */
int	mini_unset(t_mshell *minishell, t_parser *commands)
{
	int		i;
	char	*env_var;
	char	*unset_var;

	i = -1;
	if (commands->str[1] && commands->str[1][1])
	{
		unset_var = rm_quote(commands->str[1], minishell, 0);
		if (!unset_var)
			return (EXIT_FAILURE);
		while (minishell->envp[++i])
		{
			env_var = rm_quote(minishell->envp[i], minishell, 1);
			if (!ft_strncmp(env_var, unset_var, ft_strlen(unset_var)))
				return (coincidence(minishell, env_var, unset_var));
			free(env_var);
		}
		free(unset_var);
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
