/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:50:18 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/26 23:16:47 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*make_single_str(char **array, t_mshell *data)
{
	char	*result;
	int		total_len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	total_len = ft_arraylen(array);
	result = (char *)malloc(total_len + 1);
	if (!result)
		handle_error(data, 0);
	while (array[i])
	{
		ft_strcpy(result + j, array[i]);
		j += ft_strlen(array[i]);
		if (array[i + 1])
			result[j++] = ' ';
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	**normalize_str_array(char **array, t_mshell *data)
{
	char	*joined_str;
	char	**updated_array;

	joined_str = make_single_str(array, data);
	ft_free_array(array);
	updated_array = ft_split(joined_str, ' ');
	free(joined_str);
	return (updated_array);
}

// The function first checks if the command exists at the `temp` location.
// If not, it iterates through `data->paths` to locate and execute the command.
// If the command is a directory, it prints an error and exits with code 126.
// If the command is successfully executed, the function exits with code 0.
// In the case of a special variable like `$`, it handles it accordingly.
void	execute_command_in_path(int i, char *temp, char **array, t_mshell *data)
{
	char	*updated_command;

	if (!access(temp, F_OK))
	{
		if (execve(temp, array, data->envp))
		{
			ft_putstr_fd(data->commands->str[0], 2);
			ft_putendl_fd(": Is a directory", 2);
			exit(data->exit_code = 126);
		}
	}
	while (i > 0 && data->paths[i])
	{
		updated_command = ft_strjoin(data->paths[i], array[0]);
		if (!access(updated_command, F_OK))
		{
			execve(updated_command, array, data->envp);
			free(updated_command);
			exit (data->exit_code = EXIT_SUCCESS);
		}
		free(updated_command);
		i++;
	}
	if (data->commands->str[0][0] == '$' && data->commands->str[0][1])
		exit (data->exit_code = 0);
}

//Checks if PATH is set. If so, i = -42 and stops searching.
//it is helpfull for the cases when PATH is unset, cause "ls" should not work.
int	check_path_exist(t_mshell *data)
{
	int	i;

	i = 0;
	while (data->envp[i])
	{
		if (ft_strncmp(data->envp[i], "PATH", 4) == 0)
			return (-42);
		i++;
	}
	return (i);
}

//Checks if the standard commands executable exists (e.g., /bin/ls, /bin/cat)
//It also works for relative paths (e.g., ls, cat)
//If PATH is seted, i = -42. If not, commands like "ls" should not work
//therefore it prints error
//if !access, it means that access returned 0, therefore, the file exist
int	find_command(t_parser *cmd, t_mshell *data)
{
	int		i;
	char	**normalize_str;
	char	*temp;

	normalize_str = normalize_str_array(cmd->str, data);
	temp = ft_strjoin("/bin/", normalize_str[0]);
	i = check_path_exist(data);
	if (!access(normalize_str[0], F_OK))
	{
		if (execve(normalize_str[0], normalize_str, data->envp))
		{
			ft_putstr_fd(cmd->str[0], 2);
			return (ft_putendl_fd(": Is a directory", 2), 126);
		}
	}
	if (i != -42)
	{
		temp = NULL;
		return (ft_putendl_fd("minishell: no such file or directory", 2), 127);
	}
	execute_command_in_path(i, temp, normalize_str, data);
	ft_putstr_fd(cmd->str[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	ft_free_array(normalize_str);
	return (127);
}
