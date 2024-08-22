/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_single_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 19:50:18 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 17:54:53 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"

// int	check_append_outfile(t_lexer *redirections)
// {
// 	int	flags;

// 	flags = O_CREAT | O_RDWR;
// 	if (redirections->token == GREAT_GREAT)
// 		flags = O_APPEND; //esta sobreescribiendo las anteriores
// 	else
// 		flags = O_TRUNC; //esta sobreescribiendo las anteriores
// 	return (open(redirections->str, flags, 0644));
// }

char	*make_single_str(char **array, t_mshell *data)
{
	char	*result;
	int		total_len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	total_len = ft_arraylen(array);
	result = (char *)malloc(total_len + 1);
	if (!result)
		handle_error(data, 0);
	while (array[i])
	{
		ft_strcpy(result + j, array[i]);
		j += ft_strlen(array[i]);
		if (array[i + 1])
			result[j++] = ' ';
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	**normalize_str_array(char **array, t_mshell *data)
{
	char	*joined_str;
	char	**updated_array;

	joined_str = make_single_str(array, data);
	ft_free_array(array);
	updated_array = ft_split(joined_str, ' ');
	free(joined_str);
	return (updated_array);
}

//se revisa que exista el ejecutable del comando standar (ejemplo ls, cat)
//estos comandos son parte de las utilidades del sistema y se proporciona con el sistema operativo
//en el while loop se busca la ubicacion del ejecutable del comando.
int	find_command(t_parser *cmd, t_mshell *data)
{
	int		i;
	char	*updated_command;
	char	**normalize_str;
	char	*temp;

	i = 0;
	normalize_str = normalize_str_array(cmd->str, data); //para que sirve esto? por ejemplo si tengo cat << LIM.
	temp = ft_strjoin("/bin/", normalize_str[0]);
	while (data->envp[i]) //revisa si esta path
	{
		if (ft_strncmp(data->envp[i], "PATH", 4) == 0)
		{
			i = -42; //si lo encuentra i = -42
			break;
		}
		i++;
	}
	if(i != -42) //si no lo encuentra
	{
		ft_putendl_fd("minishell: no such file or directory", STDERR_FILENO);
		return (127);
	}
	if (!access(normalize_str[0], F_OK)) //! --> devuelve 0, significa que el archivo existe
	{
		//printf("check BBB\n"); //borrar
		if (execve(normalize_str[0], normalize_str, data->envp)) //$HOME no es un archivo ejecutable, es un directorio. Así que execve() fallará y entra en el if
		{
			//printf("check ccc\n"); //borrar
			ft_putstr_fd(cmd->str[0], 2);
			ft_putendl_fd(": Is a directory", 2);
			return (126);
		}
	}
	//
	if (!access(temp, F_OK)) //! --> devuelve 0, significa que el archivo existe
	{
		//printf("check BBB\n"); //borrar
		if (execve(temp, normalize_str, data->envp)) //$HOME no es un archivo ejecutable, es un directorio. Así que execve() fallará y entra en el if
		{
			//printf("check ccc\n"); //borrar
			ft_putstr_fd(cmd->str[0], 2);
			ft_putendl_fd(": Is a directory", 2);
			return (126);
		}
	}
	while (i > 0 && data->paths[i]) ///home/miguandr --> /home/home/miguandr:
	{
		updated_command = ft_strjoin(data->paths[i], normalize_str[0]); //--> /home/home/miguandr:
		if (!access(updated_command, F_OK))
		{
			execve(updated_command, normalize_str, data->envp);
			free(updated_command);
			return (EXIT_SUCCESS);
		}
		free(updated_command);
		i++;
	}
	if(cmd->str[0][0] == '$' && cmd->str[0][1])
		return (0);
	ft_putstr_fd(cmd->str[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	ft_free_array	(normalize_str);
	return (127);
}
