/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:59:12 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 22:08:04 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Identifies and returns the token type based on the provided character.
 * @c: The character to evaluate.
 *
 * This function checks if the character is a recognized token ('|', '>', '<').
 * Returns the corresponding token type (PIPE, GREAT, LESS) or 0
 * if the character is not a token.
 */
t_tokens	check_token(int c)
{
	if (c == '|')
		return (PIPE);
	else if (c == '>')
		return (GREAT);
	else if (c == '<')
		return (LESS);
	else
		return (0);
}

/**
 * Processes and identifies double-character tokens (e.g., '>>', '<<').
 * @str: The input string containing the token characters.
 * @i: The current index in the string.
 * @token: The initial token identified (either GREAT or LESS).
 * @list: Pointer to the lexer list where the new token node will be added.
 *
 * This function checks if the current token is part of a double-character token.
 * If it is, it adds a new node to the lexer list with the appropriate double
 * token (GREAT_GREAT or HERE_DOC).
 * Returns 2 if a double token is added, 0 if no double token is
 * detected, and -1 if an error occurs.
 */
static int	process_double(char *str, int i, t_tokens token, t_lexer **list)
{
	t_tokens	double_token;

	if (token == GREAT && check_token(str[i + 1]) == GREAT)
		double_token = GREAT_GREAT;
	else if (token == LESS && check_token(str[i + 1]) == LESS)
		double_token = HERE_DOC;
	else
		return (0);
	if (!add_node(NULL, double_token, list))
		return (-1);
	return (2);
}

/**
 * Handles the processing of single-character tokens and
 * identifies potential double-character tokens.
 * @str: The input string containing the tokens.
 * @i: The current index in the string.
 * @lexer_list: Pointer to the lexer list where the new token node will be added.
 *
 * This function first checks if the token is a GREAT or LESS token,
 * and if so, attempts to process it as a double-character token.
 * If a double-character token is found, it adds the appropriate node and
 * returns the corresponding result.
 * Otherwise, it adds the single-character token to the lexer list.
 * Returns the number of characters processed (1 for single, 2 for double tokens)
 * or -1 if an error occurs.
 */
int	handle_token(char *str, int i, t_lexer **lexer_list)
{
	t_tokens	token;
	int			result;

	token = check_token(str[i]);
	if (token == GREAT || token == LESS)
	{
		result = process_double(str, i, token, lexer_list);
		if (result != 0)
			return (result);
	}
	if (token)
	{
		if (!add_node(NULL, token, lexer_list))
			return (-1);
	}
	return (1);
}

/**
 * Handles the processing of a word token by determining its length and
 * adding it to the lexer list.
 * @str: The input string containing the word.
 * @start: The starting index of the word in the string.
 * @lexer_list: Pointer to the lexer list where the new word node will be added.
 *
 * This function iterates through the string from the starting index,
 * skipping quotes and whitespace,
 * until the word's length is determined. The word is then extracted and
 * added to the lexer list.
 * Returns the length of the word if successful, or -1 if an error occurs.
 */
int	handle_word(char *str, int start, t_lexer **lexer_list)
{
	int	len;

	len = 0;
	while (str[start + len] && (!check_token(str[start + len])
			&& !ft_iswhitespace(str[start + len])))
	{
		len += skip_quotes(str, start + len, '\'');
		len += skip_quotes(str, start + len, '\"');
		if (str[start + len] && (!check_token(str[start + len])
				&& !ft_iswhitespace(str[start + len])))
			len++;
	}
	if (!add_node(ft_substr(str, start, len), WORD, lexer_list))
		return (-1);
	return (len);
}
