/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sig.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 15:41:05 by ojoubout          #+#    #+#             */
/*   Updated: 2020/12/27 15:41:06 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_sigint(int sig)
{
    if (sig == SIGINT) {
        g_minishell.return_code = 1;
        if (!g_minishell.forked)
        {
            ft_putendl_fd("\b\b  \b\b", 1);
            g_minishell.return_code = 130;
            show_prompt();
        } else
            write(1, "\n", 1);
    }
}
