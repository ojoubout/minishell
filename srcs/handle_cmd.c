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
    t_cmd_list *item;

    if (g_minishell.stat != NULL)
        ft_syntax_error(str[0]);
    item = malloc(sizeof(t_cmd_list));
}


void    ft_handle_input_red(char *str)
{
    
}


void    ft_handle_output_red(char *str, int app)
{
    
}