/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 23:00:30 by marvin            #+#    #+#             */
/*   Updated: 2024/06/14 23:00:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h" //modifica el nombre

//el bash real hace distincion sobre si despues del exit viene un numero, una letra etc.
//pero las instrucciones al decir que tenemos que implementar exit sin ninguna opcion, no agregue ese manejo de errores,
//esta bien?
void	free_minishell(t_mshell *minishell) //lo necesito para alguna otra funcion?
{
	free_lexer_list(minishell->commands->redirections);
	//free_string_array(minishell->commands->str); //si agrego, me da segfault en todos los casos de exit
	free_parser_list(minishell->commands);
	free_lexer_list(minishell->lexer_list);
	free_string_array(minishell->paths);
	free_string_array(minishell->envp);
	if (minishell->pwd)
		free(minishell->pwd);
	if (minishell->old_pwd)
		free(minishell->old_pwd);
	if (minishell->pid)
		free(minishell->pid);
	if (minishell->args)
		free(minishell->args);
	// free(minishell);
}

static int	is_num(char *num)
{
	int	i;

	i = 0;
	if (num[i] == '+' || num[i] == '-')
		i++;
	while (num[i])
	{
		if (!ft_isdigit(num[i]))
			return (0);
		else
			i++;
	}
	return (1);
}

//en bash cuando tenes ; deja de leer --> exit a;b --> bash: exit: a: numeric argument required (no lo agregue)
int	mini_exit(t_mshell *minishell, t_parser *commands)
{
	if (commands == NULL || minishell == NULL)
		return (EXIT_FAILURE);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (!commands->str[1]) //si solo esta exit
		minishell->exit_code = 0;
	else if (is_num(commands->str[1]) && commands->str[2]) //si hay varios argumentos --> error
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO); //falta el exit code
		return (EXIT_FAILURE); //no libera la memoria yha que la ejecucion del programa no deberia terminar, tiene que dar la posibilidad de ingresar otro argumento
	}
	else //si solo hay un argumento --> ok
	{
		if (!is_num(commands->str[1])) //si algun caracter NO es numero //solo tiene en cuenta un + o - --> error // en bash cuando pasa esto sale del tipo bash, probar en el campus
			minishell->exit_code = handle_error2(minishell, 5, commands->str[1], NULL);
		else //si solo son numeros
			minishell->exit_code = ft_atoi(commands->str[1]);
	}
	free_minishell (minishell);
	exit (minishell->exit_code); //finaliza el programa con un código de salida específico para el sistema operativo. Puede ser utilizado para determinar cómo terminó el programa

	//return (EXIT_SUCCESS); //nunca se va a ejecutar
}
