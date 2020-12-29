/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/27 15:43:10 by ojoubout          #+#    #+#             */
/*   Updated: 2020/12/27 15:43:12 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_syntax_error(char *token)
{
    if (*token == '\n')
        token = "newline";
    else
        token[1] = 0;
    ft_fprintf(2, "minishell: syntax error near unexpected token `%s'\n",
                token);
    // ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    // ft_putstr_fd(token, 2);
    // ft_putendl_fd("'", 2);
    g_minishell.stat = 0;
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
        // ft_putstr_fd("CMD:", 1);
        cmd = g_minishell.cmd_tail->content;
        ft_lstadd_back(&cmd->argv, ft_lstnew(arg));
        // ft_putendl_fd(arg, 1);
    }
    g_minishell.read_next = NULL;
    return (0);
}

int    ft_handle_pipe(char *str)
{
    // t_list      *item;
    t_command   *cmd;

    int         p[2];
    cmd = g_minishell.cmd_tail->content;
    if (pipe(p) < 0)
        ft_error("pipe error");
    cmd->outRed = p[1];
    g_minishell.cmd_tail = ft_lstnew(ft_new_command(p[0], 1));
    ft_lstadd_back(&g_minishell.cmd_head, g_minishell.cmd_tail);

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
        // fd = open(str, O_RDONLY);
        // if (fd == -1) {
        //     ft_fprintf(2, "minishell: %s: %s\n", str, strerror(errno));
        //     g_minishell.stat = 0;
        // } else
        //     ((t_command *)g_minishell.cmd_head->content)->inRed = fd;
        ft_lstadd_back(&((t_command *)g_minishell.cmd_head->content)->inFiles, ft_lstnew(str));
        // free(str);
        g_minishell.read_next = NULL;
    } else {
        // ft_putstr_fd("IN:", 1);
        g_minishell.read_next = INPUT_RED;
        g_minishell.pos++;
    }
    return (0);
}

int    ft_handle_output_red(char *str, char *app)
{
    if (g_minishell.read_next == OUTPUT_RED)
    {
        // fd = open(str, O_RDWR | O_CREAT |
        //     (g_minishell.read_next == APP_OUTPUT_RED ? O_APPEND : 0), 0644);
        // if (fd == -1) {
        //     ft_fprintf(2, "minishell: %s: %s\n", str, strerror(errno));
        //     g_minishell.stat = 0;
        // } else
        //     ((t_command *)g_minishell.cmd_head->content)->outRed = fd;
        t_list *tmp;
        ft_lstadd_back(&((t_command *)g_minishell.cmd_head->content)->outFiles, (tmp = ft_lstnew(str)));
        // ft_fprintf(1, "%p %s %p\n", str, str, tmp);
        g_minishell.read_next = NULL;

    } else if (g_minishell.read_next == APP_OUTPUT_RED) {
        ft_lstadd_back(&((t_command *)g_minishell.cmd_head->content)->aoutFiles, ft_lstnew(str));
        g_minishell.read_next = NULL;
    } else {
        if (app == APP_OUTPUT_RED) {
            // ft_putstr_fd("AOUT:", 1);
            g_minishell.pos += 2;
        } else {
            // ft_putstr_fd("OUT:", 1);
            g_minishell.pos++;
        }
        g_minishell.read_next = app;
        // ft_putendl_fd(str, 1);
    }
    return (0);
}

void    putstr(void *str)
{
    ft_putstr_fd("|", 1);
    ft_putstr_fd(str, 1);
    ft_putendl_fd("|", 1);
}

void    print_command(void *cmd) {
    // ft_putendl_fd(cmd->argv->content);
    ft_putendl_fd("-----", 1);
    ft_lstiter(((t_command *) cmd)->argv, putstr);
}

void    print_commands()
{
    ft_lstiter(g_minishell.cmd_head, print_command);
    ft_putendl_fd("-----", 1);
}

//----------------

char    **ft_lst_to_array(t_list    *lst)
{
    char        **argv;
    int         len;
    int         i;

    len = ft_lstsize(lst);
    argv = malloc(sizeof(char *) * (len + 1));
    i = 0;
    while (i < len)
    {
        argv[i] = lst->content;
        i++;
        lst = lst->next;
    }
    argv[i] = NULL;
    return (argv);
}


void    execute_command(t_command *cmd)
{
    // t_command   *cmd;
    char        **argv;
    char        *path;

    
    // cmd = (t_command *) c;
    argv = ft_lst_to_array(cmd->argv);
    path = ft_strjoin("/bin/", argv[0]);
    char *env_args[] = { (char*)0 };
    // printf("|%s %d %d|\n", cmd->argv->content, cmd->inRed, cmd->outRed);
    open_redirect_files(cmd);
    dup2(cmd->inRed, 0);
    dup2(cmd->outRed, 1);
    execve(path, argv, env_args);
    ft_putstr_fd("minishell: command not found: ", 2);
    ft_putendl_fd(argv[0], 2);
    exit(127);
}

void    execute_commands()
{
    t_list  *lst;
    t_command   *cmd;

    lst = g_minishell.cmd_head;
    while (lst != NULL)
	{
        cmd = (t_command *)lst->content;
        if (!(cmd = (t_command *)lst->content) || !cmd->argv)
            break;
        if (fork() == 0) {
            // ft_putendl_fd("FORK", 1);
            signal(SIGINT, SIG_DFL);
            // print_command(cmd);
            execute_command(lst->content);
        }
        else
        {
            free_redirect_files();
            g_minishell.forked = 1;
            wait(&g_minishell.return_code);
            char *s = (char *)&g_minishell.return_code;
            ft_fprintf(1, "%d %d %d %d\n", s[0], s[1], s[2], s[3]);
            g_minishell.return_code = WEXITSTATUS(g_minishell.return_code);
            if (cmd->inRed != 0)
                close(cmd->inRed);
            if (cmd->outRed != 1)
                close(cmd->outRed);
            g_minishell.forked = 0;
        }
		lst = lst->next;
	}
    // ft_lstiter(g_minishell.cmd_head, execute_command);
}
