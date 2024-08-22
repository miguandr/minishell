/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 11:36:17 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:29 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void print_lexer_list(t_lexer *head) //borrar
// {
//     t_lexer *current = head;
//     if(!current){
//         printf("NULL EN LA LEXER LIST\n");
//         return;
//     }
//     while (current != NULL) {

//         printf("Str: %s, Token: %d, Index: %d\n", current->str, current->token, current->i);
//         current = current->next;
//     }
// }

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
			minishell->exit_code = ft_heredoc(commands, minishell); //si se ejecuta bien exit_code tendra valor 0, esta bien que almacene codigos de exito?
			commands->heredoc = true; //para que lo usamos????
		}
		if(commands->redirections->next)
			commands->redirections = commands->redirections->next;
		else
			break;
	}
	commands->redirections = head;
}

static int	is_main_process_builtin(int (*builtin)(t_mshell *, t_parser *)) //agregarlo a la otra funcion
{
	return (builtin == mini_cd || builtin == mini_exit
		|| builtin == mini_export || builtin == mini_unset);
}

void	execute_single_cmd(t_parser *cmd, t_mshell *data)
{
	pid_t		pid;
	int	        status;

	if (cmd && cmd->builtins && is_main_process_builtin(cmd->builtins)) //mejor poner todo aca junto
	{
		data->exit_code = cmd->builtins(data, cmd);
		return ;
	}
	check_heredoc(data, cmd);
	pid = fork();
	if (pid < 0)
		handle_error(data, 5);
	else if (pid == 0) //antes estaba if
	{
		execute_command(data, cmd); //en un mismo comando pueden haber mas de un tipo de redireccion
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_code = WEXITSTATUS(status);
}

//If there are multiple commands, creates a pipe to connect current and next.
//Every time communication between two processes is needed using a pipe:
// - Declare an array of two int to store the read and write ends of the pipe.
// - Use the pipe() function to initialize the pipe.
//If there is a PIPE token, spawns a child process.
int	execute_pipe_cmd(t_mshell *minishell)
{
	int			fd[2];
	int			fd_prev;
	t_parser	*temp_commands;

	temp_commands = minishell->commands;
	fd_prev = STDIN_FILENO;
	while (temp_commands)
	{
		if (temp_commands->next)
		{
			if (pipe(fd) == -1)
				return (handle_error(minishell, 7));
		}
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
		// close(fd[1]);
		// if (temp_commands->prev)
		// 	close(fd_prev);
		// fd_prev = get_fd(minishell, fd, temp_commands);
		//temp_commands = temp_commands->next; //le saque los pasos innecesarios, ver si funciona todo bien
		// if (temp_commands->next) //porque es esto necesario?
		// 	temp_commands = temp_commands->next;
		// else
		// 	break;
	}
	wait_childspid(minishell, minishell->pid);
	return (EXIT_SUCCESS);
}

//data->pipes + 1 = cfubre todos los procesos mientras que el +1 adicional es comúnmente utilizado para guardar un valor extra. Puede ser para propósitos de seguridad, alineación de memoria, o para almacenar un valor especial (como un PID extra o un valor sentinela).
//data->pid sera un array de enteros para los pid de cada proceso + 1 espacio extra
int	executor(t_mshell *data)
{
	signal(SIGINT, handle_ctrl_c_child);
	signal(SIGQUIT, handle_ctrl_backslash);
	data->in_cmd = 1; //ver si necesitamos esto

	if (data-> pipes == 0)
	{
		execute_single_cmd(data->commands, data);
	}
	else
	{
		data->pid = ft_calloc(sizeof(int), data->pipes + 2);
		if (!data->pid)
			return (handle_error(data, 1));
		execute_pipe_cmd(data);
	}
	data->in_cmd = 0; //ver si necesitamos esto
	return (EXIT_SUCCESS);
}
