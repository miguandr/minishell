/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:57:14 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:57:14 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	cd(char *str, t_mshell *minishell);

// Searchs in the env the variable HOME= or OLDPWD=
// if they dont exist anymore (e.g. because of 'unset'), error.
static int	change_directory(char **env, char *str)
{
	int		i;
	char	*new;

	i = -1;
	new = NULL;
	while (env[++i])
	{
		if (ft_strncmp(env[i], str, ft_strlen(str)) == 0)
		{
			new = ft_strdup(env[i] + ft_strlen(str));
			break ;
		}
	}
	if (chdir(new) != 0)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(" not set", 2);
		free(new);
		return (1);
	}
	if (!ft_strncmp("OLDPWD=", str, ft_strlen(str)))
		ft_putendl_fd(new, 1);
	free(new);
	return (0);
}

// cd ..
static void	ft_previous(t_mshell *minishell)
{
	int		last_slash;
	char	*pwd;
	char	*prev;
	int		i;

	pwd = minishell->pwd;
	i = 0;
	while (pwd[i])
	{
		if (pwd[i] == '/')
			last_slash = i;
		i++;
	}
	prev = ft_substr(pwd, 0, last_slash);
	chdir(prev);
	free(prev);
}

// Function to handle the remaining path after ".."
static void	ft_handle_remainder(char *str, int i, t_mshell *minishell)
{
	char	*temp;

	if (str[i] && str[i] == '/')
	{
		change_pwd(minishell);
		i++;
		if (str[i])
		{
			temp = ft_strdup(str + i);
			cd(temp, minishell);
			free(temp);
		}
	}
}

// Main cd function, handles ".." and general directory changes
int	cd(char *str, t_mshell *minishell)
{
	int	i;

	i = 0;
	if (!ft_strncmp(str, "..", 2))
	{
		i += 2;
		ft_previous(minishell);
		ft_handle_remainder(str, i, minishell);
	}
	else
	{
		if (ft_strncmp(str, ".", 1) && chdir(str) != 0)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

// CD only acepts one additional argument, if there are more, error.
// if no arguments, changes directoy to HOME.
// if '-' argument, changes directoy to the OLD PWD.
// if '..' argument, changes to previos directory.
// if '.' argument, nothing happens.
int	mini_cd(t_mshell *minishell, t_parser *commands)
{
	if (commands->str[1] && commands->str[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	else if (!commands->str[1])
		change_directory(minishell->envp, "HOME=");
	else if (!ft_strncmp(commands->str[1], "-", 1))
		change_directory(minishell->envp, "OLDPWD=");
	else
	{
		if (cd(commands->str[1], minishell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}	
	change_pwd(minishell);
	return (0);
}
