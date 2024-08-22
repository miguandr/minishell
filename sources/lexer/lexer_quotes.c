/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:19:25 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:08:41 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Finds the next occurrence of a specific quote character in a string.
 * @str: The input string to search through.
 * @start: The starting index to begin the search.
 * @target: The quote character to search for.
 *
 * This function searches for the next occurrence of the target quote character
 * in the string starting from the given index. It skips escaped quotes.
 * Returns the index of the found quote, or -1 if no matching quote is found.
 */
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

/**
 * Counts the total number of quotes in a string and checks if they are balanced.
 * @str: The input string to evaluate.
 *
 * This function iterates through the string to count single and double quotes.
 * It ensures that each type of quote is paired correctly.
 * Returns 1 if the quotes are balanced, or 0 if they are not.
 */
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

/**
 * Skips over a quoted segment in a string, returning the length of the segment.
 * @str: The input string containing the quoted segment.
 * @start: The starting index of the quote character.
 * @quote: The type of quote to skip (single or double).
 *
 * This function determines the length of the quoted segment in the string,
 * including both the opening and closing quotes.
 * Returns the total length of the quoted segment, including the quotes.
 */
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
