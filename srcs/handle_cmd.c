#include "minishell.h"
#include <stdio.h>

int ft_syntax_error(char *token)
{
    if (*token == '\n')
        token = "newline";
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    ft_putstr_fd(token, 2);
    ft_putendl_fd("'", 2);
    g_minishell.stat = 2;
    return (1);
}

int    ft_handle_cmd(char *str)
{
    int	len;
    char *arg;
    t_command   *cmd;

    len = ft_word_length(str, SEP);
    arg = ft_substr(str, 0, len);
    if (g_minishell.read_next == INPUT_RED)
        ft_handle_input_red(arg);
    else if (g_minishell.read_next == OUTPUT_RED || g_minishell.read_next == APP_OUTPUT_RED)
        ft_handle_output_red(arg, g_minishell.read_next);
    else {
        ft_putstr_fd("CMD:", 1);
        cmd = g_minishell.cmd_head->content;
        ft_lstadd_back(&cmd->argv, ft_lstnew(arg));
        ft_putendl_fd(arg, 1);
    }
    g_minishell.read_next = NULL;
    return (0);
}

int    ft_handle_pipe(char *str)
{
    t_list      *item;
    t_command   *cmd;

    int         p[2];
    cmd = g_minishell.cmd_head->content;
    if (pipe(p) < 0)
        ft_error("pipe error");
    cmd->outRed = p[1];
    item = ft_lstnew(ft_new_command(p[0], 1));
    ft_lstadd_back(&g_minishell.cmd_head, item);

    ft_putstr_fd("PIPE:", 1);
    ft_putendl_fd(str, 1);
    g_minishell.read_next = NULL;
    g_minishell.pos++;
    return (0);
}


int    ft_handle_input_red(char *str)
{
    if (g_minishell.read_next == INPUT_RED)
    {
        ft_putendl_fd(str, 1);
        free(str);
        g_minishell.read_next = NULL;
    } else {
        ft_putstr_fd("IN:", 1);
        g_minishell.read_next = INPUT_RED;
        g_minishell.pos++;
    }
    return (0);
}


int    ft_handle_output_red(char *str, char *app)
{
    if (g_minishell.read_next == OUTPUT_RED || g_minishell.read_next == APP_OUTPUT_RED)
    {
        ft_putendl_fd(str, 1);
        free(str);
        g_minishell.read_next = NULL;
    } else {
        if (app == APP_OUTPUT_RED) {
            ft_putstr_fd("AOUT:", 1);
            g_minishell.pos += 2;
        } else {
            ft_putstr_fd("OUT:", 1);
            g_minishell.pos++;
        }
        g_minishell.read_next = app;
        // ft_putendl_fd(str, 1);
    }
    return (0);
}