/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 23:14:59 by miguandr          #+#    #+#             */
/*   Updated: 2024/07/22 23:14:59 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// unset: not enough arguments (revisar si comportamiento de solo unset en el campus)
// verificar si unset zz/ genera error de unset: not a valid identifier
// verificar si unset zzz= genera error de unset: not a valid identifier
static char	*rm_quote(char *str, t_mshell *minishell, int flag)
{
	int		i;
	char	*temp;
	char	*substr;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			if (flag == 1)
			{
				substr = ft_substr(str, 0, i);
				temp = delete_quotes(substr, minishell); //si uno todo en la misma linea funciona pero da memory leaks
				free(substr);
				return (temp);
			}
			else
				return (NULL);
		}
		i++;
	}
	temp = delete_quotes(str, minishell); //si no hay = de todos modos hay que darle valor a var name
	return (temp);
}

static char	**newarray(char **envp, char *str) //no borrar porque no es la misma que utils
{
	char	**new_array;
	int		i;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	new_array = ft_calloc((i), sizeof(char*));
	if (!new_array)
		return (NULL); //cambiar
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], str, ft_strlen(str)))
			i++;
		if (envp[i])
			new_array[j++] = ft_strdup(envp[i++]);
	}
	return (new_array);
}

static int coincidence (t_mshell *minishell, char *env_var, char *unset_var)
{
	char	**temp;

	temp = newarray(minishell->envp, env_var);
	free_string_array(minishell->envp);
	minishell->envp = temp;
	free(env_var);
	free(unset_var);
	return (EXIT_SUCCESS);
}

int	mini_unset(t_mshell *minishell, t_parser *commands)
{
	int		i;
	char	*env_var;
	char	*unset_var;

	i = -1;
	if (commands->str[1] && commands->str[1][1])
	{
		unset_var = rm_quote(commands->str[1], minishell, 0);
		if (!unset_var)
			return(EXIT_FAILURE); //cambiar por handle error
		while (minishell->envp[++i])
		{
			env_var = rm_quote(minishell->envp[i], minishell, 1); //separa de env pwd=lalal en solo pwd
			if (!ft_strncmp(env_var, unset_var, ft_strlen(unset_var))) //buscar en el array envp si se encuentra la variable definida con export
				return (coincidence(minishell, env_var, unset_var));
			free(env_var);
		}
		free(unset_var);
		return (EXIT_SUCCESS); //si no hay coincidencia no pasa nada
	}
	return (EXIT_SUCCESS); // si solo esta unset, no pasa nada
}


//si solo tengo unset no hace nada
//export zzz=abc
//unset zzz / "zzz" / 'zzz' (ok)
//unset zzz=abc / zzz= / zzzz / ZZZZ (NO)