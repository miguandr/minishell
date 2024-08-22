/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 17:29:08 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:31:24 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"

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
