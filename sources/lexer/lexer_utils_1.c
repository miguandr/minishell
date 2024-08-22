/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 22:44:12 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:41:28 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	skip_space(char *str, int i)
{
	while (str[i] == ' ' || (str[i] > 8 && str[i] < 14))
		i++;
	return (i);
}

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
