/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 18:49:16 by marvin            #+#    #+#             */
/*   Updated: 2024/07/14 18:49:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"



//abrimos el archivo output.txt en modo escritura. Si el archivo no existe, se crea.
//O_TRUNC asegura que el archivo se vacía si ya existe.
//O_APPEND se asegura que se una todo quede inexado, osea no reemplaza
//0644 es el modo de archivo que da permisos de lectura y escritura al propietario, y sólo de lectura al grupo y a otros.
int ft_great (t_parser *commands, t_mshell *minishell)
{
	char *output;
	int file;

	output = commands->redirections->str;
	if(commands->redirections->token == GREAT)
		file = open(output, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(commands->redirections->token == GREAT_GREAT)
		file = open(output, O_RDWR | O_CREAT | O_APPEND, 0644);
	if(file < 0)
		return(handle_error(minishell, 8));
	if (dup2(file, STDOUT_FILENO) < 0)
	{
		close(file);
		return (handle_error(minishell, 8));
	}
	close(file);
	return(EXIT_SUCCESS);
}

int ft_less (t_mshell *minishell, char *input)
{
	int file;

	file = open(input, O_RDONLY);
	if(file < 0)
		return(handle_error(minishell, 8));
	if (dup2(file, STDIN_FILENO) < 0) //file > 0 
	{
		close(file);
		return (handle_error(minishell, 8));
	}
	close(file);
	return(EXIT_SUCCESS);
}

// void print_lexer_list(t_lexer *head) { //borrar
//     t_lexer *current = head;
//     if(!current)
//     {
//         printf("NULL EN LA LEXER LIST\n");
//         return;
//     }
//     while (current != NULL) 
//     {
//         printf("Str: %s, Token: %d, Index: %d\n", current->str, current->token, current->i);
//         current = current->next;
//     }
// }


int ft_redirections (t_parser *commands, t_mshell *minishell)
{
	t_lexer  *head;
	head = commands->redirections;
	//print_lexer_list(commands->redirections); //borrar
	while(commands->redirections)
	{
		if(commands->redirections->token == GREAT || commands->redirections->token == GREAT_GREAT)
		{
			if(ft_great(commands, minishell) != 0)
				return(EXIT_FAILURE);
		}
		else if(commands->redirections->token == LESS)
		{
			if(ft_less(minishell, commands->redirections->str))
				return(EXIT_FAILURE);
		}
		else if(commands->redirections->token == HERE_DOC)
		{
			if(ft_less(minishell, commands->hd_file_name))
				return(EXIT_FAILURE);
		}
		commands->redirections = commands->redirections->next; //le saque los pasos innecesarios, ver si funciona todo bien
		// if (commands->redirections->next) // /bin/cat < txt.txt > prueba.txt
		// {
		// 	printf("hay next\n"); //borrar
		// 	commands->redirections = commands->redirections->next;
		// }
		// else
		// {
		// 	printf("no hay next???\n"); //borrar //ATENCION no esta detectando cuando hay mas de una redireccion. entonces ejecuta la misma redireccion una y otra vez si no le pongo el break
		// 	break;
		// }
	}
	commands->redirections = head;
	return(EXIT_SUCCESS);
}

//first executes the redirecction if there are any
//second executes the builtin if there are any. if not, check is there are
//any other command like cat, ls, etc.
//The exit command is necessary as it only affects the child process;
// it needs to be finished.
void execute_command(t_mshell *minishell, t_parser *commands)
{
	if (commands->num_redirections) // antes estaba commands->num_redirections
	{
		minishell->exit_code = ft_redirections(commands, minishell);
		//printf("check 01\n"); //borrar
	}
	if (commands->builtins)
	{
		//printf("check 02\n"); //borrar
		minishell->exit_code = commands->builtins(minishell, commands);
	}
	else if (commands->str[0][0])
	{
		//printf("check 03\n"); //borrar
		minishell->exit_code = find_command(commands, minishell);
		//printf("check 03.2\n"); //borrar  NO LLEGA ACA
	}
	//printf("check 04\n"); //borrar
	exit(minishell->exit_code); //es necesario, este exit es parte del proceso hijo y solo lo afecta a el
}


//Redirect the file descriptors from which file will be read / stored in
// the case of a previous or subsequent command, respectively.
//Close(fd[0]) to close the read end of the pipe.
//Close(fd[1]) to close the write end of the pipe.
//Ends the child process and go back to the parent process
int child_process(t_mshell *minishell, t_parser *commands, int fd[2], int fd_prev)
{
	if (commands->prev && dup2(fd_prev, STDIN_FILENO) < 0)
		return (handle_error(minishell, 7));
	close(fd[0]);
	if (commands->next && dup2(fd[1], STDOUT_FILENO) < 0)
		return (handle_error(minishell, 7));
	close(fd[1]);
	if(fd_prev)
		close(fd_prev);
	execute_command(minishell, commands);
	exit (minishell->exit_code);
}

//Forks a child process to execute a command.
int ft_fork(t_mshell *minishell, t_parser *commands, int fd[2], int fd_prev)
{
	static int i = 0; //no ponerlo en lineas separadas

	//i = 0;
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
	return(EXIT_SUCCESS);
}

//Recorre todos los PIDs de los procesos hijos y espera a que terminen
//Esto asegura que el proceso padre espere a que todos los procesos hijos terminen antes de continuar.
//El código de salida refleja el estado del último comando ejecutado.
//WIFEXITED(status): Verifica si el hijo terminó normalmente.
//WEXITSTATUS(status): Obtiene el código de salida del hijo.
int wait_childspid (t_mshell *minishell, int *array)
{
	int i;
	int status;

	i = 0;
	while(array[i])
	{
		waitpid(array[i], &status, 0);
		i++;
	}
	if(WIFEXITED(status))
		minishell->exit_code = WEXITSTATUS(status);
	return(EXIT_SUCCESS);
}

//Esta función juega un papel crucial al determinar de dónde debe leer el proceso hijo su entrada estándar (STDIN) antes de ejecutar un comando.
//Dependiendo de la configuración de tools->heredoc, decide entre leer de un heredoc o del extremo de lectura del pipe (end[0]).
int	get_fd(t_mshell *minishell, int end[2], t_parser *commands)
{
	int fd;

	if(commands->heredoc)
	{
		close(end[0]);
		fd = open(commands->hd_file_name, O_RDONLY);
		if (fd < 0)
			return (handle_error(minishell, 7));
	}
	else
		fd = end[0];
	return(fd);
}
