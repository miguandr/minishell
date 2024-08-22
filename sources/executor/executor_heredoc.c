/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 19:27:45 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/22 20:57:41 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	exp_var_hd(t_mshell *data, char *str, char *result, int result_len)
{
	char	*var_name;
	char	*var_value;
	int		var_name_len;
	int		var_value_len;

	var_value_len = 0;
	var_name = get_variable_name(str + 1, data);
	var_name_len = ft_strlen(var_name);
	var_value = get_variable_value(data, var_name);
	if (var_value)
	{
		var_value_len = ft_strlen(var_value);
		ft_strcpy(result + result_len, var_value);
		result_len += var_value_len;
		free(var_value);
	}
	free(var_name);
	return (var_name_len + 1);
}

static char	*expand_heredoc(t_mshell *data, char *str)
{
	char	*result;
	int		result_len;
	int		i;
	int		str_len;

	i = 0;
	result_len = 0;
	str_len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	if (!result)
		handle_error(data, 0);
	while (i < str_len)
	{
		if (str[i] == '$')
		{
			i += exp_var_hd(data, str + i, result, result_len);
			result_len = ft_strlen(result);
		}
		else
			result[result_len++] = str[i++];
	}
	result[result_len] = '\0';
	return (result);
}

//crea el contenido de heredoc_content pero no hace nada si dentro hay funciones
int	ft_heredoc(t_parser *commands, t_mshell *minishell) //(minishell, cmd->redirections, cmd->hd_file_name);
{
	char	*input_line;
	char	*delimiter;
	int		file;
	char	*expanded_line;

	//printf("entro a heredoc");
	delimiter = commands->redirections->str;
	file = open(commands->hd_file_name, O_RDWR | O_CREAT | O_TRUNC, 0644); //abro el hd file para escribirle data
	if (file < 0)
		return (handle_error(minishell, 8));
	// if (dup2(file, STDOUT_FILENO) < 0)
	// {
	// 	close(file);
	// 	return (handle_error(minishell, 8));
	// }


	//input_line = readline("> ");


	while (1)//a;adir expander aca (ver si esta bien el que a;adi) // ver manejo de quotes, supuestamente habria que eliminarlas
	{
		input_line = readline("> ");
		if (!input_line)
			return (EXIT_FAILURE); //CAMBIAR, NO HABRIA QUE AGREGARLO EN EL LEXER TAMBIEN?
		if (!ft_strncmp(input_line, delimiter, ft_strlen(delimiter)) && input_line[ft_strlen(delimiter)] == '\0') //si encuentra el limitador se termina
		{
			free(input_line);
			break ;
		}
		else
		{
			expanded_line = expand_heredoc(minishell, input_line); //revisar flag
			if (expanded_line)
				ft_putendl_fd(expanded_line, file); //o deberia ser putstr? ver bien O_APPEND
			free(expanded_line);
			//free(input_line);
		}
		free(input_line);
	}
	close(file);
	return (EXIT_SUCCESS);
}

char	*generate_name(void)
{
	static int	i;
	char		*file_num;
	char		*file_name;

	i = 0;
	file_num = ft_itoa(i++);
	file_name = ft_strjoin("/tmp/heredoc_", file_num);
	free(file_num);
	return (file_name);
}
