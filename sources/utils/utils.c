/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:29:08 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:02:50 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Duplicates a NULL-terminated array of strings.
 * @array: The array of strings to duplicate.
 *
 * This function duplicates each string in the provided array, allocating
 * memory for a new array. If any allocation fails, it frees all previously
 * allocated memory and returns NULL. Returns a pointer to the duplicated array.
 */
char	**dup_str(char **array)
{
	char	**result;
	size_t	i;

	i = 0;
	if (array == NULL)
		return (NULL);
	while (array[i] != NULL)
		i++;
	result = ft_calloc(sizeof(char *), i + 1);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (array[i])
	{
		result[i] = ft_strdup(array[i]);
		if (result[i] == NULL)
		{
			ft_free_array(result);
			return (NULL);
		}
		i++;
	}
	return (result);
}

/**
 * Frees and clears a linked list of parser nodes.
 * @lst: Double pointer to the head of the linked list of t_parser nodes.
 *
 * This function iterates through a linked list of t_parser nodes, freeing
 * all dynamically allocated memory associated with each node, including
 * the command strings, redirections, and heredoc file names. After freeing,
 * it sets the list pointer to NULL.
 */
void	ft_commands_clear(t_parser **lst)
{
	t_parser	*tmp;
	t_lexer		*redirections_tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		redirections_tmp = (*lst)->redirections;
		free_lexer_list(redirections_tmp);
		if ((*lst)->str)
			ft_free_array((*lst)->str);
		if ((*lst)->hd_file_name)
			free((*lst)->hd_file_name);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
