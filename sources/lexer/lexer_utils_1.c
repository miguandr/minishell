/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 22:44:12 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:02:34 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Skips over whitespace characters in a string.
 * @str: The input string.
 * @i: The starting index in the string.
 *
 * This function increments the index to skip spaces and other whitespace
 * characters (tabs, newlines, etc.) in the string. It returns the updated index.
 */
int	skip_space(char *str, int i)
{
	while (str[i] == ' ' || (str[i] > 8 && str[i] < 14))
		i++;
	return (i);
}

/**
 * Returns the last node in a linked list of lexer nodes.
 * @list: Pointer to the head of the linked list of t_lexer nodes.
 *
 * This function iterates through the list to find and return the last node.
 * If the list is empty, it returns NULL.
 */
t_lexer	*list_last(t_lexer *list)
{
	t_lexer	*temp;

	if (!list)
		return (NULL);
	temp = list;
	while (temp->next)
		temp = temp->next;
	return (temp);
}

/**
 * Creates a new lexer node.
 * @str: The string to be stored in the node.
 * @token: The token type associated with the string.
 *
 * This function allocates memory for a new t_lexer node, sets its
 * string, token, index, and initializes the next and previous pointers
 * to NULL. It returns a pointer to the new node,
 * or NULL if memory allocation fails.
 */
t_lexer	*lexer_new_node(char *str, int token)
{
	static int	index = 0;
	t_lexer		*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->token = token;
	new_node->i = index++;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/**
 * Adds a new lexer node to the end of a linked list.
 * @list: Double pointer to the head of the linked list of t_lexer nodes.
 * @new_node: The new lexer node to be added.
 *
 * This function appends a new lexer node to the end of the linked list.
 * If the list is empty, the new node becomes the first node in the list.
 */
void	lexer_add_last(t_lexer **list, t_lexer *new_node)
{
	t_lexer	*last;

	last = NULL;
	if (!list || !new_node)
		return ;
	if (*list == NULL)
		*list = new_node;
	else
	{
		last = list_last(*list);
		last->next = new_node;
		new_node->prev = last;
	}
}

/**
 * Adds a new lexer node to the lexer list.
 * @str: The string to be stored in the new node.
 * @token: The token type associated with the string.
 * @lexer_list: Double pointer to the head of the lexer list.
 *
 * This function creates a new lexer node and adds it to the lexer list.
 * If memory allocation fails, it returns 0. Otherwise, it returns 1.
 */
int	add_node(char *str, t_tokens token, t_lexer **lexer_list)
{
	t_lexer	*node;

	node = lexer_new_node(str, token);
	if (!node)
		return (0);
	if (*lexer_list == NULL)
		*lexer_list = node;
	else
		lexer_add_last(lexer_list, node);
	return (1);
}
