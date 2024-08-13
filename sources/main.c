/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:31:22 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/13 23:12:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/header_mig.h"

// static void print_lexer_list(t_lexer *head) //borrar
// {
//     t_lexer *current = head;
//     if(!current){
//         printf("NULL EN LA LEXER LIST\n");
//         return;
//     }
//     while (current != NULL) {

//         printf("Str: %s, Token: %d, Index: %d\n", current->str, current->token, current->i);
//         current = current->next;
//     }
// }

/*void print_parser_node(t_parser *node) {
    if (node == NULL) {
        return;
    }
    printf("\nParser Node:\n");
    if (node->str != NULL) {
        for (int i = 0; node->str[i] != NULL; i++) {
            printf("  str[%d]: %s\n", i, node->str[i]);
        }
    }
    if(!node->prev)
        printf("  PREV NULL\n");
    else
        printf("  PREV not NULL ⚠️\n");
    if(!node->next)
        printf("  NEXT NULL:\n");
    else
        printf("  NEXT not NULL ⚠️\n");
    printf("  num_redirections: %d\n", node->num_redirections);
    printf("  hd_file_name: %s\n", node->hd_file_name);
    printf("  Redirections:\n");
    print_lexer_list(node->redirections);
}

// Función para imprimir la lista enlazada t_parser
void print_parser_list(t_parser *head) {
    t_parser *current = head;
    while (current != NULL) {
        print_parser_node(current);
        current = current->next;
    }
}*/

int	minishell(t_mshell *data)
{
	lexer(data);
	parser(data);
	executor(data);
	reset_data(data);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_mshell	data;

	if (ac != 1 || av[1])
	{
		printf("Minishell does not accept arguments\n");
		exit(0);
	}
	data.envp = dup_str(envp);
	get_pwd(&data);
	init_data(&data);
	// printf("MAKE WELCOME MESSAGE/n"); //MAKE
	// lexer(&data);
	// parser(&data);
	// executor(&data);
	// reset_data(&data);
	minishell(&data);
	return (0);
}
