/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:43:09 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:38:29 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Tokenizes the input arguments of the minishell session.
 * @data: Pointer to the mshell structure containing arguments and lexer list.
 *
 * This function processes the arguments provided by the user, skipping spaces
 * and checking for tokens or words. It calls appropriate handler functions
 * based on whether a token or a word is encountered. If any handler returns a
 * negative value, the tokenization fails and the function returns 0. Otherwise,
 * it returns 1 upon successful tokenization.
 */
int	tokenizer(t_mshell *data)
{
	int	i;
	int	handled;

	i = 0;
	while (data->args[i])
	{
		i = skip_space(data->args, i);
		handled = 0;
		if (check_token(data->args[i]))
			handled = handle_token(data->args, i, &data->lexer_list);
		else
			handled = handle_word(data->args, i, &data->lexer_list);
		if (handled < 0)
			return (0);
		i += handled;
	}
	return (1);
}

/**
 * Reads user input, processes it, and tokenizes it for the minishell session.
 * @data: Pointer to the mshell structure containing arguments and lexer list.
 *
 * This function prompts the user for input, trims leading and trailing spaces,
 * and assigns the trimmed input to the mshell structure. If the input is empty,
 * it resets the data and exits if necessary. The function adds the input to the
 * command history, checks for balanced quotes, and calls the tokenizer function
 * to process the input. It also performs error checking on the lexer list.
 * Returns 1 on successful processing, or handles errors appropriately.
 */
int	lexer(t_mshell *data)
{
	const char	*prompt;
	char		*user_input;
	char		*trimmed_input;

	prompt = "minishell> ";
	user_input = readline(prompt);
	trimmed_input = ft_strtrim(user_input, " ");
	free(user_input);
	data->args = trimmed_input;
	if (!data->args)
	{
		ft_putendl_fd("exit", 1);
		exit(EXIT_SUCCESS);
	}
	if (*data->args == '\0')
		return (reset_data(data));
	add_history(data->args);
	data->lexer_list = NULL;
	if (!count_quotes(data->args))
		return (handle_error(data, 1));
	if (!tokenizer(data))
		return (handle_error(data, 0));
	lexer_error_check(data->lexer_list, data);
	return (1);
}
