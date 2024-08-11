/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguandr <miguandr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 16:58:50 by miguandr          #+#    #+#             */
/*   Updated: 2024/08/11 16:59:02 by miguandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/header_mig.h"



char *expand_str(t_mshell *data, char *str, bool *flag)
{
    char *expanded_str = NULL;

    if (ft_strchr(str, '\"'))
    {
        expanded_str = expand_double_quote(data, str);
    }
    else if (ft_strchr(str, '$'))
    {
        expanded_str = expand_variable_helper(data, str, flag);
    }
    else if (ft_strchr(str, '\''))
    {
        expanded_str = remove_single_quote(str, data);
    }
    else
    {
        expanded_str = ft_strdup(str);  // Duplicate the string if no expansion needed
    }

    return expanded_str;
}
