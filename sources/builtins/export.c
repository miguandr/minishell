/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtorrett <dtorrett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 20:58:05 by dtorrett          #+#    #+#             */
/*   Updated: 2024/08/22 20:58:05 by dtorrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//si se ingresa solo export, sin ninguna otra variable, o si esa variable es "" (solo el caracter nulo) imprime el enviroment normal
//export VAR1
//si ejecuto expot solo, ahora voy a tener esa nueva variable
//export VAR1=1
//al ejecutar export --> VAR1="1"
//Esta variable se convierte en parte del entorno del shell y estará disponible para cualquier proceso hijo que se ejecute desde ese shell.
//export PWD=hola   --> va a actualizar el valor que tiene pwd

//revisar si el comando tiene =. si lo tiene:
//separar el comando por =
//buscar la primera parte en el array del envp
//si lo encuentra, actualizar el valor
//si no lo encuentra lo a;axde al final

static int	check_coincidence(t_mshell *minishell, int i, char *var_name, char *add_var)
{

	if (!ft_strncmp(minishell->envp[i], var_name, ft_strlen(var_name)))
	{
		free(minishell->envp[i]);
		minishell->envp[i] = add_var;
		return (0);
	}
	return (1);
}

// Verifica si el carácter c es válido como parte de un identificador de variable en Bash
static bool	check_valid_identifier(char c)
{
	if ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		c == '_' )
		return (true);
	else
		return (false);
}

//elimina todos los quotes
//si encuentra =, almacena el nombre de la variable sin quotes en var_name
static char	*check_quotes(char *str, char **var_name, t_mshell *minishell)
{
	int		i;
	char	*temp;
	char	*substr;

	i = 0;
	temp = delete_quotes(str, minishell);
	while (str[i])
	{
		if (str[i] == '=')
		{
			substr = ft_substr(str, 0, i);
			*var_name = delete_quotes(substr, minishell); //export varname & =
			free(substr);
			return(temp);
		}
		else
			i++;
	}
	*var_name = delete_quotes(str, minishell); //si no hay = de todos modos hay que darle valor a var name
	return(temp);
}

static int	error_check(t_mshell *minishell, t_parser *commands)
{
	int	i;

	i = 0;
	//printf("check 01\n"); //borrar.
	// if (commands->str[1] && commands->str[2])
	// {
	// 	printf("check 02\n"); //borrar.
	// 	return (handle_error2(minishell, 1, NULL, commands->str));
	// }
	// else 
	if (commands->str[1])
	{
		//printf("check 2222\n"); //borrar.
		//printf("str %d\n", commands->str[1][i]); //borar
		if (ft_isdigit(commands->str[1][0]) || commands->str[1][0] == '=')
		{
			//printf("check 03\n"); //borrar.
			return (handle_error2(minishell, 2, commands->str[1], NULL)); //sacar las llaves
		}
		while (commands->str[1][i] != '=' /*&& commands->str[1][i]*/)
		{
			//printf("check 3333\n"); //borrar.
			if (!check_valid_identifier(commands->str[1][i]))
			{
				//printf("check 444444\n"); //borrar.
				if (commands->str[1][i] == '!')
				{
					//printf("check 04\n"); //borrar.
					return (handle_error2(minishell, 3, commands->str[1] + i, NULL));
				}
				else
				{
					//printf("check 05\n"); //borrar
					return (handle_error2(minishell, 4, commands->str[1], NULL));
				}
			}
			i++;
			if (!commands->str[1][i])
				break;
		}
	}
	//printf("check 10\n"); //borrar.
	return (EXIT_SUCCESS);
}

//str[0] --> export
//str[1] --> nombre de la variable (puede incluir la definicion)
//str[2] --> error ya que significa que el comando original tenia espacios
int	mini_export(t_mshell *minishell, t_parser *commands)
{
	int		i;
	char	**temp;
	char	*add_var;
	char	*var_name;

	i = -1;
	if (error_check(minishell, commands))
	{
		//printf("check 01\n");
		return (EXIT_FAILURE); //wsacar las llaves
	}
	if (!commands->str[1] || commands->str[1][0] == '\0')
		mini_env(minishell, commands);
	else
	{
		add_var = check_quotes(commands->str[1], &var_name, minishell); //elimina los quotes de la variable (lo que esta antes de =)
		while(minishell->envp[++i])
		{
			if(check_coincidence(minishell, i, var_name, add_var) == 0)
				return(EXIT_SUCCESS);
		}
		temp = new_array(minishell->envp, add_var);
		free_string_array(minishell->envp);
		minishell->envp = temp;
		free(var_name);
	}
	return (EXIT_SUCCESS);
}
