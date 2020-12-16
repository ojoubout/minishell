#include "minishell.h"

void ft_syntax_error(char *token)
{
    if (token == '\n')
        token = "newline";
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    ft_putstr_fd(token, 2);
    ft_putendl_fd("'", 2);
    exit(1);
}

void    ft_handle_cmd(char *str)
{
    if (g_minishell.stat != NULL)
        ft_syntax_error(str[0]);

}

void    ft_handle_pipe(char *str)
{
    t_list *item;
    t_command   *cmd;
    int         p[2];
    cmd = g_minishell.cmd_head->content;
    if (g_minishell.stat != NULL || cmd->argv == NULL)
        ft_syntax_error(str[0]);
    if (pipe(p) < 0)
        ft_error("pipe error");
    cmd->outRed = p[1];
    item = ft_lstnew(ft_new_command(p[0], p[1]));
    ft_lstadd_back(&g_minishell.cmd_head, item);

}


void    ft_handle_input_red(char *str)
{
    
}


void    ft_handle_output_red(char *str, int app)
{
    
}