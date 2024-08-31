/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 11:36:17 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/31 13:00:36 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	check_heredoc(t_mshell *minishell, t_parser *commands)
{
	t_lexer	*head;

	head = commands->redirections;
	while (commands && commands->redirections)
	{
		if (commands->redirections->token == HERE_DOC)
		{
			if (commands->hd_file_name)
				free(commands->hd_file_name);
			commands->hd_file_name = generate_name();
			minishell->exit_code = ft_heredoc(commands, minishell);
			commands->heredoc = true;
		}
		if (commands->redirections->next)
			commands->redirections = commands->redirections->next;
		else
			break ;
	}
	commands->redirections = head;
}

static void	execute_single_cmd(t_parser *cmd, t_mshell *data)
{
	pid_t	pid;
	int		status;

	if (cmd && cmd->builtins && (cmd->builtins == mini_cd
			|| cmd->builtins == mini_exit || cmd->builtins == mini_export
			|| cmd->builtins == mini_unset))
	{
		data->exit_code = cmd->builtins(data, cmd);
		return ;
	}
	check_heredoc(data, cmd);
	pid = fork();
	if (pid < 0)
		handle_error(data, 5);
	else if (pid == 0)
		execute_command(data, cmd);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_code = WEXITSTATUS(status);
}

static int	create_pipe_if_needed(t_parser *command, int fd[2])
{
	if (command->next)
	{
		if (pipe(fd) == -1)
			return (-1);
	}
	return (0);
}

//If there are multiple commands, creates a pipe to connect current and next.
//Every time communication between two processes is needed using a pipe:
// - Declare an array of two int to store the read and write ends of the pipe.
// - Use the pipe() function to initialize the pipe.
//If there is a PIPE token, spawns a child process.
static int	execute_pipe_cmd(t_mshell *minishell)
{
	int			fd[2];
	int			fd_prev;
	t_parser	*temp_commands;

	temp_commands = minishell->commands;
	fd_prev = STDIN_FILENO;
	while (temp_commands)
	{
		if (create_pipe_if_needed(temp_commands, fd) != 0)
			return (handle_error(minishell, 7));
		check_heredoc(minishell, temp_commands);
		ft_fork(minishell, temp_commands, fd, fd_prev);
		if (temp_commands->next)
			close(fd[1]);
		if (fd_prev != STDIN_FILENO)
			close(fd_prev);
		if (temp_commands->next)
			fd_prev = fd[0];
		else
			fd_prev = STDIN_FILENO;
		temp_commands = temp_commands->next;
	}
	return (wait_childspid(minishell, minishell->pid), EXIT_SUCCESS);
}

//data->pipes + 1 = cfubre todos los procesos mientras que el +1 adicional es
//comúnmente utilizado para guardar un valor extra. Puede ser para propósitos
//de seguridad, alineación de memoria, o para almacenar un valor especial
//(como un PID extra o un valor sentinela).
//data->pid sera un array de enteros para los pid de cada proceso + 1
//espacio extra
int	executor(t_mshell *data)
{
	signal(SIGINT, handle_ctrl_c_child);
	signal(SIGQUIT, handle_ctrl_backslash);
	if (data-> pipes == 0)
		execute_single_cmd(data->commands, data);
	else
	{
		data->pid = ft_calloc(sizeof(int), data->pipes + 2);
		if (!data->pid)
			return (handle_error(data, 1));
		execute_pipe_cmd(data);
	}
	return (EXIT_SUCCESS);
}
