/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoumani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 14:07:08 by hmoumani          #+#    #+#             */
/*   Updated: 2021/01/01 14:07:13 by hmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int to_skip(char *s)
{
    int i;

    i = 1;
    if (s[0] != '-')
        return (0);
    while (s[i])
    {
        if (s[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

void ft_echo(char **argv)
{
    int i;

    if (!argv[1])
    {
        ft_putstr_fd("\n", 1);
        return ;
    }
    i = 1;
    while (to_skip(argv[i]))
    {
        i++;
    }
    while (argv[i])
    {
        ft_putstr_fd(argv[i], 1);
        i++;
        if (argv[i])
            ft_putstr_fd(" ", 1);
    }
    if (strcmp(argv[1], "-n"))
        ft_putstr_fd("\n", 1);
}