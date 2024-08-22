/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:10:27 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 21:34:38 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks the first and last tokens in the lexer list for validity.
 * @list: Pointer to the head of the lexer list.
 *
 * This function checks if the first and last tokens in the lexer list
 * are valid. The first token must be either a WORD or HERE_DOC, and the
 * last token must be a WORD. It returns 1 if valid, otherwise 0.
 */
static int	check_edges(t_lexer *list)
{
	if ((list->token != WORD && list->token != HERE_DOC)
		|| list_last(list)->token != WORD)
		return (0);
	return (1);
}

/**
 * Checks the lexer list for invalid token sequences.
 * @list: Pointer to the head of the lexer list.
 *
 * This function iterates through the lexer list to check for invalid
 * sequences where two consecutive tokens are not of type WORD. It returns 1
 * if all sequences are valid, otherwise 0.
 */
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

/**
 * Performs error checking on the lexer list.
 * @lexer_list: Pointer to the head of the lexer list.
 * @data: Pointer to the t_mshell structure for handling errors.
 *
 * This function checks the lexer list for validity. If the list is empty
 * or if invalid tokens are found at the edges or in the sequence, it calls
 * handle_error with the appropriate error code.
 */
void	lexer_error_check(t_lexer *lexer_list, t_mshell *data)
{
	if (!lexer_list)
		EXIT_SUCCESS ;
	if (!check_edges(lexer_list))
		handle_error(data, 2);
	if (!check_invalid_sequence(lexer_list))
		handle_error(data, 3);
}
