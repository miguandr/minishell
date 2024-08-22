/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 22:54:42 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 22:54:42 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parser	*parser_new_node(t_mshell *minishell)
{
	t_parser	*new_node;

	new_node = (t_parser *)malloc(sizeof(t_parser));
	if (!new_node)
		handle_error(minishell, 0);
	new_node->num_redirections = 0;
	new_node->redirections = NULL;
	new_node->heredoc = false;
	new_node->hd_file_name = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->flag = false;
	return (new_node);
}

void	parser_add_last(t_parser **head, t_parser *new)
{
	t_parser	*tmp;

	tmp = *head;
	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

//(temp->prev && temp->next) if there is one node before and after
//(temp->prev == NULL && temp->next) if it is the first node
//(temp->prev && temp->next == NULL) if it is the last node
void	ft_delnode(t_lexer *temp, t_lexer **head)
{
	if (temp->prev == NULL && temp->next == NULL)
	{
		free(temp->str);
		free(temp);
		return ;
	}
	if (temp->prev && temp->next)
	{
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
	}
	if (temp->prev == NULL && temp->next)
	{
		*head = temp->next;
		temp->next->prev = NULL;
	}
	if (temp->prev && temp->next == NULL)
	{
		temp->prev->next = NULL;
	}
	free(temp->str);
	free(temp);
}
