/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:57:56 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:57:56 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//first executes the redirecction if there are any
//second executes the builtin if there are any. if not, check is there are
//any other command like cat, ls, etc.
//The exit command is necessary as it only affects the child process;
// it needs to be finished.
void	execute_command(t_mshell *minishell, t_parser *commands)
{
	if (commands->num_redirections)
		minishell->exit_code = ft_redirections(commands, minishell);
	if (commands->builtins)
		minishell->exit_code = commands->builtins(minishell, commands);
	else if (commands->str[0][0])
		minishell->exit_code = find_command(commands, minishell);
	exit(minishell->exit_code);
}

//Redirect the file descriptors from which file will be read / stored in
// the case of a previous or subsequent command, respectively.
//Close(fd[0]) to close the read end of the pipe.
//Close(fd[1]) to close the write end of the pipe.
//Ends the child process and go back to the parent process
static int	child_process(t_mshell *data, t_parser *commands,
			int fd[2], int fd_prev)
{
	if (commands->prev && dup2(fd_prev, STDIN_FILENO) < 0)
		return (handle_error(data, 7));
	close(fd[0]);
	if (commands->next && dup2(fd[1], STDOUT_FILENO) < 0)
		return (handle_error(data, 7));
	close(fd[1]);
	if (fd_prev)
		close(fd_prev);
	execute_command(data, commands);
	exit (data->exit_code);
}

//Forks a child process to execute a command.
int	ft_fork(t_mshell *minishell, t_parser *commands, int fd[2], int fd_prev)
{
	static int	i = 0;

	if (minishell->reset == 1)
	{
		minishell->reset = 0;
		i = 0;
	}
	minishell->pid[i] = fork();
	if (minishell->pid[i] < 0)
		return (handle_error(minishell, 5));
	else if (minishell->pid[i] == 0)
		child_process(minishell, commands, fd, fd_prev);
	i++;
	return (EXIT_SUCCESS);
}

// Iterates over all child process PIDs and waits for them to finish.
// IT ensures that the parent process waits for all child processes
//to complete before continuing.
// The exit code reflects the status of the last command executed.
// WIFEXITED(status): Checks if the child process terminated normally.
// WEXITSTATUS(status): Retrieves the exit code of the child process.
int	wait_childspid(t_mshell *minishell, int *array)
{
	int	i;
	int	status;

	i = 0;
	while (array[i])
	{
		waitpid(array[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status))
		minishell->exit_code = WEXITSTATUS(status);
	return (EXIT_SUCCESS);
}
