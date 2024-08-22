/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:10:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:43:09 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_edges(t_lexer *list)
{
	if ((list->token != WORD && list->token != HERE_DOC)
		|| list_last(list)->token != WORD)
		return (0);
	return (1);
}

static int	check_invalid_sequence(t_lexer *list)
{
	while (list && list->next)
	{
		if (list->token != WORD && list->next->token != WORD)
			return (0);
		list = list->next;
	}
	return (1);
}

void	lexer_error_check(t_lexer *lexer_list, t_mshell *data)
{
	if (!lexer_list)
		EXIT_SUCCESS ;
	if (!check_edges(lexer_list))
		handle_error(data, 2);
	if (!check_invalid_sequence(lexer_list))
		handle_error(data, 3);
}
