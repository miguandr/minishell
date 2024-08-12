/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 14:54:25 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/11 17:33:34 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"


// char	*expand_double_quote(t_mshell *data, char *str)
// {
// 	char	*result;
// 	int		len;
// 	int		i;
// 	int		result_len;
// 	char	*var_value;

// 	i = 0;
// 	result_len = 0;
// 	len = ft_strlen(str);
// 	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
// 	if (!result)
// 		handle_error(data, 0);

// 	while (i < len)
// 	{
// 		if (str[i] == '\"')
// 		{
// 			i++;
// 			while (i < len && str[i] != '\"')
// 			{
// 				if (str[i] == '$')
// 				{
// 					var_value = expand_variable(data, str + i, &i);
// 					if (var_value)
// 					{
// 						ft_strcpy(result + result_len, var_value);
// 						result_len += ft_strlen(var_value);
// 						free(var_value);
// 					}
// 				}
// 				else if (str[i] == '\'')
// 				{
// 					result[result_len] = str[i];
// 					result_len++;
// 					i++;
// 					while (i < len && str[i] != '\'')
// 					{
// 						if (str[i] == '$')
// 						{
// 							var_value = expand_variable(data, str + i, &i);
// 							if (var_value)
// 							{
// 								ft_strcpy(result + result_len, var_value);
// 								result_len += ft_strlen(var_value);
// 								free(var_value);
// 							}
// 						}
// 						else
// 						{
// 							result[result_len] = str[i];
// 							result_len++;
// 							i++;
// 						}
// 					}
// 					if (str[i] == '\'')
// 					{
// 						result[result_len] = str[i];
// 						result_len++;
// 						i++;
// 					}
// 				}
// 				else
// 				{
// 					result[result_len] = str[i];
// 					result_len++;
// 					i++;
// 				}
// 			}
// 			if (str[i] == '\"')
// 				i++;
// 		}
// 		else
// 		{
// 			result[result_len] = str[i];
// 			result_len++;
// 			i++;
// 		}
// 	}
// 	result[result_len] = '\0';
// 	return (result);
// }

// char	*expand_str(t_mshell *data, char *str)
// {
// 	if (ft_strchr(str, '\'') != NULL)
// 		return (single_quote_helper(str, data));
// 	else if (ft_strchr(str, '\"') != NULL)
// 		return (expand_double_quote_helper(data, str));
// 	else if (ft_strchr(str, '$') != NULL)
// 		return (expand_variable_helper(data, str));
// 	return (str);
// }

// void	expander(t_mshell *data, char **str)
// {
// 	char	*expanded_str;
// 	int		i;

// 	i = 0;
// 	while (str[i] != NULL)
// 	{
// 		expanded_str = expand_str(data, str[i]);
// 		if (expanded_str != str[i])
// 		{
// 			free(str[i]);
// 			str[i] = expanded_str;
// 		}
// 		i++;
// 	}
// }

/*
char	*handle_inside_quote(t_mshell *data, char *s, int *i, char *result)
{
	char	*var_value;
	int		result_len;

	result_len = ft_strlen(result);
	while (s[*i] && s[*i] != '\"')
	{
		if (s[*i] == '$')
		{
			var_value = expand_variable(data, s + *i, i);
			if (var_value)
			{
				ft_strcpy(result + result_len, var_value);
				result_len += ft_strlen(var_value);
				free (var_value);
			}
		}
		else
			result[result_len++] = s[(*i)++];
	}
	result[result_len] = '\0';
	return (result);
}

char	*expand_double_quote(t_mshell *data, char *str)
{
	char	*result;
	int		len;
	int		i;
	int		result_len;

	if (ft_strstr(str, "\"export\""))
		return (ft_strdup(str));
	i = 0;
	result_len = 0;
	len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	while (i < len)
	{
		if (str[i] == '\"')
		{
			i++;
			result = handle_inside_quote(data, str, &i, result);
			if (str[i] == '\"')
				i++;
			result_len += ft_strlen(result);
		}
		else
			result[result_len++] = str[i++];
	}
	return (result);
}*/

/*int main() {
    t_mshell data;
    data.envp = (char **)malloc(5 * sizeof(char *));
    data.envp[0] = strdup("VAR1=value1");
    data.envp[1] = strdup("VAR2=value2");
    data.envp[2] = strdup("HOME=/home/user");
    data.envp[3] = strdup("export");
    data.envp[4] = NULL;

    data.last_exit_status = 42;

    char **commands = (char **)malloc(13 * sizeof(char *));
    commands[0] = strdup("echo \"$HOME\"");
    commands[1] = strdup("echo 'Value is $VAR1'");
    commands[2] = strdup("echo \"Values: $VAR1 $VAR2\"");
    commands[3] = strdup("echo \"$NON_EXISTENT_VAR\"");
    commands[4] = strdup("echo NoQuotesHere");
    commands[5] = strdup("echo 'Only single quotes here'");
    commands[6] = strdup("echo \"Variable at the start: $VAR1\"");
    commands[7] = strdup("echo \"Variable in the middle: text $VAR1 text\"");
    commands[8] = strdup("echo \"Variable at the end: $VAR1\"");
    commands[9] = strdup("echo \"Multiple variables: $VAR1 $VAR2 $HOME\"");
    commands[10] = strdup("echo \"export\"");
    commands[11] = strdup("echo \"$?\"");
    commands[12] = NULL;

    printf("Original Commands:\n");
    for (int i = 0; commands[i] != NULL; i++) {
        printf("%s\n", commands[i]);
    }
    printf("\n");

    expander(&data, commands);

    printf("Expanded Commands:\n");
    for (int i = 0; commands[i] != NULL; i++) {
        printf("%s\n", commands[i]);
    }
    printf("\n");

    ft_free_array(commands);
    ft_free_array(data.envp);

    return 0;
}*/



//old version
char	*expand_double_quote(t_mshell *data, char *str)
{
	char	*result;
	int		len;
	int		i;
	int		result_len;
	char	*var_value;

	i = 0;
	result_len = 0;
	len = ft_strlen(str);
	result = ft_calloc(MAX_EXP_SIZE, sizeof(char));
	if (!result)
		handle_error(data, 0);

	while (i < len)
	{
		if (str[i] == '\"')
		{
			i++;
			while (i < len && str[i] != '\"')
			{
				if (str[i] == '$')
				{
					var_value = expand_variable(data, str + i, &i);
					if (var_value)
					{
						ft_strcpy(result + result_len, var_value);
						result_len += ft_strlen(var_value);
						free(var_value);
					}
				}
				else if (str[i] == '\'')
				{
					result[result_len] = str[i];
					result_len++;
					i++;
					while (i < len && str[i] != '\'')
					{
						if (str[i] == '$')
						{
							var_value = expand_variable(data, str + i, &i);
							if (var_value)
							{
								ft_strcpy(result + result_len, var_value);
								result_len += ft_strlen(var_value);
								free(var_value);
							}
						}
						else
						{
							result[result_len] = str[i];
							result_len++;
							i++;
						}
					}
					if (str[i] == '\'')
					{
						result[result_len] = str[i];
						result_len++;
						i++;
					}
				}
				else
				{
					result[result_len] = str[i];
					result_len++;
					i++;
				}
			}
			if (str[i] == '\"')
				i++;
		}
		else
		{
			result[result_len] = str[i];
			result_len++;
			i++;
		}
	}
	result[result_len] = '\0';
	return (result);
}

/*new version*/
/*char	*expand_str(t_mshell *data, char *str, bool *flag)
{
	char	*expanded_str;

	expanded_str = NULL;
	if (ft_strchr(str, '\"') != NULL && str[0] != '\'')
		expanded_str = expand_double_quote(data, str);
	else if (ft_strchr(str, '$') != NULL)
		expanded_str = expand_variable_helper(data, str, flag);
	else if (ft_strchr(str, '\'') != NULL)
		expanded_str = remove_single_quote(str, data);
	else
		expanded_str = ft_strdup(str);  // Duplicate the string if no expansion needed
	return expanded_str;  // Caller must free expanded_str
}*/

char	*expand_str(t_mshell *data, char *str, bool *flag)
{
	if (ft_strchr(str, '\"') != NULL && str[0] != '\'')
	{
		return (expand_double_quote(data, str));
	}
	else if (ft_strchr(str, '$') != NULL)
		return (expand_variable_helper(data, str, flag));
	else if (ft_strchr(str, '\'') != NULL)
	{
		//printf("no entreeeees 1 \n");
		return (remove_single_quote(str, data));
	}
	return (str);
}

void	expander(t_mshell *data, char **str, bool *flag)
{
	char	*expanded_str;
	int		i;


	i = 0;
	while (str[i] != NULL)
	{
		expanded_str = expand_str(data, str[i], flag);
		if (expanded_str != str[i])
		{
			free(str[i]);
			str[i] = expanded_str;
		}
		i++;
	}
}

