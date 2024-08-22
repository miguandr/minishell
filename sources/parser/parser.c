/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 16:21:41 by marvin            #+#    #+#             */
/*   Updated: 2024/06/07 16:21:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h" //modifica el nombre



static bool	is_quoted(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			return (true);
		i++;
	}
	return (false);
}
void handle_redirection(t_lexer *redirections)
{
	t_lexer	*current;
	int		i;
	int		j;

	current = redirections;
	while (current)
	{
		if (is_quoted(current->str))
		{
			i = 0;
			j = 0;
			while (current->str[i])
			{
				if (current->str[i] != '\"' && current->str[i] != '\'')
					current->str[j++] = current->str[i];
				i++;
			}
			current->str[j] = '\0';
		}
		current = current->next;
	}
}





//Counts the amount of argument of WORD tokens before the PIPE
//There should not be more redirections.
//If a non-WORD token is encountered, handles the error and returns.
static int	count_args(t_lexer *head, t_mshell *minishell)
{
	int		amount;
	t_lexer	*current;

	amount = 0;
	current = head;
	if (!current)
		return (handle_error(minishell, 4));
	while (current && current->token != PIPE)
	{
		if (current -> token != WORD)
			return (handle_error(minishell, 4));
		amount++;
		current = current->next;
	}
	return (amount);
}

//Counts the number of WORD tokens in lexer_list.
//Allocates memory with calloc for the arguments array.
//Copies str from lexer L to the argument array, deleting nodes from lexer L.
//Expands the first element of the array (builtin name).
static char	**built_args(t_mshell *minishell, int i, bool *flag)
{
	t_lexer	*current;
	t_lexer	*next_node;
	int		arguments;
	//char	**expanded_array;
	char	**arg_array;
	
	current = minishell->lexer_list;
	arguments = count_args(minishell->lexer_list, minishell); //al nodo general le a;ade los token WORD
	arg_array = calloc ((arguments + 1), sizeof(char*));
	if (!arg_array)
	{
		handle_error (minishell, 0);
		return (NULL);
	}
	while (i < arguments)
	{
		arg_array[i] = strdup(current->str);
		next_node = current->next;
		ft_delnode(current, &minishell->lexer_list);
		i++;
		current = next_node;
	}
	minishell->lexer_list = current;
	//minishell->commands->flag = false; //ver
	expander(minishell, arg_array, flag);
	//expanded_array = expander_builtins(minishell, arg_array);
	return (arg_array);
}

//Iterates over each node in the lexer_list until a PIPE token is found.
//For each redirection token, creates and adds a new node to the list.
//Deletes processed nodes from lexer_list and updates the current node.
//Increments the number of redirections.
//Sets the array of WORD-strings using built_args.
//Assigns the built-in function name and pointer using builtins_handle
static void	built_node(t_parser *commands, t_mshell *minishell, bool *flag)
{
	t_lexer	*current;
	t_lexer	*next_node;
	t_lexer	*node;

	current = minishell->lexer_list;
	while (current && current->token != PIPE)
	{
		if (current->token != WORD)
		{
			node = lexer_new_node(strdup(current->next->str), current->token );
			lexer_add_last(&commands->redirections, node);
			next_node = current->next->next;
			ft_delnode(current->next, &minishell->lexer_list);
			ft_delnode(current, &minishell->lexer_list);
			current = next_node;
			commands->num_redirections++;
		}
		else
			current = current->next;
	}

	//new 01
	if (commands->redirections)
		handle_redirection(commands->redirections);


	commands->str = built_args(minishell, 0, flag);
	commands->builtins = builtins_handler(commands->str[0]);
}

//Iterates over each node in the lexer_list until it is empty.
//For each simple command, creates and builds a new node.
//If the token is a PIPE, increments pipes and deletes it from lexer_list.
void	parser(t_mshell *minishell)
{
	t_parser	*node;
	t_mshell	*current;

	current = minishell;
	minishell->commands = NULL;

	while (current->lexer_list)
	{
		node = parser_new_node(minishell);
		built_node (node, minishell, &node->flag);
		parser_add_last(&minishell->commands, node);
		if (current->lexer_list && current->lexer_list->token == PIPE)
		{
			minishell->pipes++;
			ft_delnode(current->lexer_list, &minishell->lexer_list);
		}
		//current = minishell; //es necesario??? parece que no
	}

	/*************FREES**********************/
	//no hay que ponerlo ahora, es solo a fin de chequeo de leaks
	//free_parser_list(minishell->commands);
	// if(minishell->envp)
	//     free_string_array(minishell->envp);
	//free(minishell);


	//free_string_array MIGUE YA TIENE ESTA FUNCION EN LIBFT
	//cambiar luego por ft_free_array
}
