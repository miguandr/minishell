/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:49:02 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:49:02 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_array(char **array, int i)
{
	while (array[i])
	{
		ft_putstr_fd(array[i], STDERR_FILENO);
		i++;
		if (array[i])
			write(2, " ", 1);
	}
}

char	**new_array(char **array, char *str)
{
	char	**new_array;
	int		i;

	i = 0;
	while (array[i])
		i++;
	new_array = ft_calloc((i + 2), sizeof(char *));
	if (!new_array)
	{
		return (NULL);
	}
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = str;
	return (new_array);
}

char	*delete_quotes(char *str, t_mshell *minishell)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	result = ft_calloc((len + 1), sizeof(char));
	if (!result)
	{
		handle_error(minishell, 0);
		return (NULL);
	}
	while (i < len)
	{
		if (str[i] != '\'' && str[i] != '\"')
			result[j++] = str[i];
		i++;
	}
	return (result);
}

// Rewrites the PWD and OLDPWD variables in the envp array.
void	change_envp(t_mshell *minishell)
{
	int		i;
	char	*temp;

	i = 0;
	while (minishell->envp[i])
	{
		if (ft_strncmp(minishell->envp[i], "PWD=", 4) == 0)
		{
			temp = ft_strjoin("PWD=", minishell->pwd);
			free(minishell->envp[i]);
			minishell->envp[i] = temp;
		}
		if (ft_strncmp(minishell->envp[i], "OLDPWD=", 7) == 0)
		{
			temp = ft_strjoin("OLDPWD=", minishell->old_pwd);
			free(minishell->envp[i]);
			minishell->envp[i] = temp;
		}
		i++;
	}
}

// Rewrites the PWD and OLDPWD variables in the t_mshell struct
void	change_pwd(t_mshell *minishell)
{
	char	*temp;

	temp = ft_strdup(minishell->pwd);
	free(minishell->old_pwd);
	minishell->old_pwd = temp;
	free(minishell->pwd);
	minishell->pwd = getcwd(NULL, 0);
	change_envp(minishell);
}
