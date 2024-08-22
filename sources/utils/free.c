/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:37:48 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:37:48 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Frees a linked list of lexer nodes.
 * @list: Pointer to the head of the linked list of t_lexer nodes.
 *
 * This function iterates through a linked list of t_lexer nodes,
 * freeing the memory allocated for each node's string and the node itself.
 */
void	free_lexer_list(t_lexer *list)
{
	t_lexer	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->str);
		free(temp);
	}
}

/**
 * Frees a NULL-terminated array of strings.
 * @array: The array of strings to be freed.
 *
 * This function iterates through the array of strings, freeing each string
 * and setting its pointer to NULL. It then frees the array itself.
 */
void	free_string_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		++i;
	}
	free(array);
}

/**
 * Frees a linked list of parser nodes.
 * @list: Pointer to the head of the linked list of t_parser nodes.
 *
 * This function iterates through a linked list of t_parser nodes,
 * freeing the memory allocated for each node's strings, redirections,
 * and the node itself.
 */
void	free_parser_list(t_parser *list)
{
	t_parser	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free_string_array(temp->str);
		free_lexer_list(temp->redirections);
		free(temp);
	}
}
