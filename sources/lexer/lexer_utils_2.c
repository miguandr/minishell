/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:19:04 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:38:45 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/******BORRAR LUEGO TODO EL ARCHIVO******/

t_lexer	*lexer_delete_one(t_lexer **list)
{
	t_lexer	*node;

	if (!list || !*list)
		return (NULL);
	node = *list;
	*list = node->next;
	if (node->str)
	{
		free(node->str);
		node->str = NULL;
	}
	free(node);
	return (NULL);
}

void	lexer_delete_first(t_lexer **list)
{
	t_lexer	*node;

	if (!list || !*list)
		return ;
	node = *list;
	*list = node->next;
	if (*list)
		(*list)->prev = NULL;
	lexer_delete_one(&node);
}

/*Ver si realmente es necesaria, ya hay ft_delnode en pareser_utils
Ver si en algun momento necesitamos el index realmente*/
void	lexer_delete_specific(t_lexer **list, int node_index)
{
	t_lexer	*node;
	t_lexer	*prev;

	if (!list || !*list)
		return ;
	node = *list;
	prev = NULL;
	while (node && node->i != node_index)
	{
		prev = node;
		node = node->next;
	}
	if (!node)
		return ;
	if (prev != NULL)
		prev->next = node->next;
	else
		*list = node->next;
	if (node->next)
		node->next->prev = prev;
	lexer_delete_one(&node);
}

void	lexer_delete_all(t_lexer **list)
{
	t_lexer	*current;
	t_lexer	*temp;

	if (!list || !*list)
		return ;
	current = *list;
	while (current)
	{
		temp = current->next;
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		free(current);
		current = temp;
	}
	*list = NULL;
}
