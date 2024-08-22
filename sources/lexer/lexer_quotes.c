/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:19:25 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:41:41 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_next_quote(char *str, int start, char target)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (str[i] == target && (i == 0 || str[i - 1] != '\\'))
			return (i);
		i++;
	}
	return (-1);
}

int	count_quotes(char *str)
{
	int		i;
	int		single_quote;
	int		double_quote;

	i = -1;
	single_quote = 0;
	double_quote = 0;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			i = find_next_quote(str, i + 1, str[i]);
			if (i == -1)
				return (0);
			if (str[i] == '\'')
				single_quote += 2;
			else if (str[i] == '"')
				double_quote += 2;
		}
	}
	if (single_quote == 0 || (single_quote > 0 && single_quote % 2 == 0)
		|| (double_quote == 0 || (double_quote > 0 && double_quote % 2 == 0)))
		return (1);
	return (0);
}

int	skip_quotes(const char *str, int start, char quote)
{
	int	len;

	len = 0;
	if (str[start] == quote)
	{
		len++;
		start++;
		while (str[start + len] && str[start + len] != quote)
			len++;
		if (str[start + len] == quote)
			len++;
	}
	return (len);
}
