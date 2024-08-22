/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 19:10:18 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 21:15:06 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Sets the current working directory (PWD) or old working directory (OLDPWD).
 * @env_var: The environment variable string containing the path.
 * @pwd: Pointer to the string where the path will be stored.
 * @prefix_len: Length of the prefix before the actual path in the
 *  environment variable.
 *
 * This function duplicates the path from the environment variable and assigns it
 * to the pwd pointer. If a previous value exists, it frees the memory.
 * Returns 1 if successful, otherwise returns 0.
 */
static int	set_pwd(char *env_var, char **pwd, int prefix_len)
{
	if (*pwd)
		free(*pwd);
	*pwd = ft_strdup(env_var + prefix_len);
	return (*pwd != NULL);
}

/**
 * Retrieves the current and old working directory paths from the environment.
 * @data: Pointer to the mshell structure containing the environment variables.
 *
 * This function iterates through the environment variables to find and set
 *  the current working directory (PWD) and the old working directory (OLDPWD)
 * in the mshell structure.
 */
void	get_pwd(t_mshell *data)
{
	int	i;

	i = 0;
	data->pwd = NULL;
	data->old_pwd = NULL;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PWD=", 4) == 0)
			set_pwd(data->envp[i], &(data->pwd), 4);
		if (ft_strncmp(data->envp[i], "OLDPWD=", 7) == 0)
			set_pwd(data->envp[i], &(data->old_pwd), 7);
		i++;
	}
}

/**
 * Retrieves the PATH environment variable.
 * @envp: The environment variables array.
 *
 * This function searches for the PATH variable in the
 * environment variables array.
 * If found, it returns a substring of the PATH value. If not found,
 * it returns an empty string.
 *
 * Returns: A dynamically allocated string containing the PATH value
 *  or an empty string.
 */
char	*get_path(char **envp)
{
	char	*path_var;

	path_var = ft_strstr(*envp, "PATH=");
	if (!path_var)
		return (ft_strdup(""));
	return (ft_substr(path_var + 5, 0, ft_strlen(path_var + 5)));
}

/**
 * Processes the PATH environment variable and stores paths in mshell structure.
 * @data: Pointer to the mshell structure containing the environment variables.
 *
 * This function retrieves the PATH environment variable, splits it into
 * individual paths, and stores them in the mshell structure. It also ensures
 * each path ends with a '/'.
 *
 * Returns: EXIT_SUCCESS on success, EXIT_FAILURE on failure.
 */
int	handle_envp(t_mshell *data)
{
	char	*path;
	char	*temp;
	size_t	len;
	int		i;

	i = 0;
	path = get_path(data->envp);
	if (!path)
		return (EXIT_FAILURE);
	data->paths = ft_split(path, ':');
	free(path);
	if (!data->paths)
		return (EXIT_FAILURE);
	while (data->paths[i])
	{
		len = ft_strlen(data->paths[i]);
		if (len > 0 && data->paths[i][len - 1] != '/')
		{
			temp = ft_strjoin(data->paths[i], (char const *) '/');
			free(data->paths[i]);
			data->paths = &temp;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
