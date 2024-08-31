/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 23:22:42 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/31 13:03:22 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//Opens the file output.txt in write mode. If it does not exist, it is created.
//O_TRUNC ensures that the file is emptied if it already exists.
//O_APPEND ensures that all writes are appended to the end of the file,
// not replacing existing content.
//0644 is the file mode that gives read and write permissions to the owner,
// and read-only permissions to the group and others.
static int	ft_great(t_parser *commands, t_mshell *minishell)
{
	char	*output;
	int		file;

	output = commands->redirections->str;
	if (commands->redirections->token == GREAT)
		file = open(output, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (commands->redirections->token == GREAT_GREAT)
		file = open(output, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (file < 0)
		return (handle_error(minishell, 8));
	if (dup2(file, STDOUT_FILENO) < 0)
	{
		close(file);
		return (handle_error(minishell, 8));
	}
	close(file);
	return (EXIT_SUCCESS);
}

static int	ft_less(t_mshell *minishell, char *input)
{
	int	file;

	file = open(input, O_RDONLY);
	if (file < 0)
	{
		minishell->exit_code = 1;
		reset_data(minishell);
		return (minishell->exit_code);
	}
	if (dup2(file, STDIN_FILENO) < 0)
	{
		close(file);
		return (handle_error(minishell, 8));
	}
	close(file);
	return (EXIT_SUCCESS);
}

int	ft_redirections(t_parser *commands, t_mshell *minishell)
{
	t_lexer	*head;

	head = commands->redirections;
	while (commands->redirections)
	{
		if (commands->redirections->token == GREAT
			|| commands->redirections->token == GREAT_GREAT)
		{
			if (ft_great(commands, minishell) != 0)
				return (EXIT_FAILURE);
		}
		else if (commands->redirections->token == LESS)
		{
			if (ft_less(minishell, commands->redirections->str))
				return (EXIT_FAILURE);
		}
		else if (commands->redirections->token == HERE_DOC)
		{
			if (ft_less(minishell, commands->hd_file_name))
				return (EXIT_FAILURE);
		}
		commands->redirections = commands->redirections->next;
	}
	commands->redirections = head;
	return (EXIT_SUCCESS);
}
