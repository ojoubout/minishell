/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 18:01:25 by ojoubout          #+#    #+#             */
/*   Updated: 2020/12/27 18:01:26 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     open_file(char *file, int flags, char *type)
{
    int         fd;
    t_command   *cmd;
    int         red;

    cmd = g_minishell.cmd_head->content;
    red = type == INPUT_RED ? cmd->inRed : cmd->outRed;
    if (red != 1)
        close(red);
    if (type == INPUT_RED)
        fd = open(file, flags);
    else
        fd = open(file, flags, 0644);
    
    if (fd == -1) {  
        ft_fprintf(2, "minishell: %s: %s\n", file, strerror(errno));
        g_minishell.stat = 0;
    }
    return fd;
}

void    open_input_file(void  *f)
{
    int         fd;
    char        *file;
    t_command   *cmd;

    file = f;
    cmd = g_minishell.cmd_head->content;
    fd = open_file(file, O_RDONLY, INPUT_RED);
    if (fd != -1)
        cmd->inRed = fd;
}

void    open_out_file(void  *f)
{
    int         fd;
    char        *file;
    t_command   *cmd;

    file = f;
    cmd = g_minishell.cmd_head->content;
    fd = open_file(file, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_RED);
    if (fd != -1)
        cmd->outRed = fd;
}

void    open_aout_file(void  *f)
{
    int         fd;
    char        *file;
    t_command   *cmd;

    file = f;
    cmd = g_minishell.cmd_head->content;
    fd = open_file(file, O_WRONLY | O_CREAT | O_APPEND, APP_OUTPUT_RED);
    if (fd != -1)
        cmd->outRed = fd;
}

void ft_free(void *ptr)
{
    free(ptr);
}
void    open_redirect_files()
{
    ft_lstiter(((t_command *)g_minishell.cmd_head->content)->inFiles, open_input_file);
    ft_lstiter(((t_command *)g_minishell.cmd_head->content)->outFiles, open_out_file);
    ft_lstiter(((t_command *)g_minishell.cmd_head->content)->aoutFiles, open_aout_file);
    ft_lstclear(&((t_command *)g_minishell.cmd_head->content)->inFiles, ft_free);
    ft_lstclear(&((t_command *)g_minishell.cmd_head->content)->outFiles, ft_free);
    ft_lstclear(&((t_command *)g_minishell.cmd_head->content)->aoutFiles, ft_free);
}
