/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 15:42:21 by ojoubout          #+#    #+#             */
/*   Updated: 2020/12/30 15:42:23 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *env_sep = "'\"$\\";

char    *ft_convert_env(char *str)
{
    int     i;
    int     on_dollar;
    char    *res;
    char    *tmp;
    char    var[100];
    int     len;

    i = 0;
    on_dollar = 0;
    res = ft_strdup("");
    ft_bzero(var, 100);
    while (str[i])
    {
        if (ft_on_char(str, i, "$")) {
            on_dollar = ++i;
            // len = get_next_word(&str[i], env_sep);
            len = ft_word_length(str + i, env_sep);
            tmp = ft_substr(str, i, len);
            i += len;
            sprintf(var, "{%s}", tmp);
            free(tmp);
            tmp = res;
            res = ft_strjoin(res, var);
            free(tmp);
        } else {
            tmp = res;
            res = ft_strappend(res, str[i]);
            free(tmp);
            i++;
        }
    }
    return (res);
}

char    *ft_get_var(char *name)
{
    char *res;
    t_list *lst;

    lst = g_env.env_head;
    while (lst)
    {
        
    }
}