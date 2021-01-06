/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoumani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 14:59:15 by hmoumani          #+#    #+#             */
/*   Updated: 2020/12/29 14:59:18 by hmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


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

t_list *ft_array_to_lst(char **array)
{
    t_list *head;
    int i;

    // if (!(head = malloc(sizeof(t_list *))))
    //     return (NULL);
    i = 0;
    head = NULL;
    while (array[i])
    {
        ft_lstadd_back(&head, ft_lstnew(ft_strdup(array[i])));
        i++;
    }
    return (head);
}

int is_command(char *s)
{
    if (ft_strequ(s, "echo"))
        return (1);
    if (ft_strequ(s, "cd"))
        return (2);
    if (ft_strequ(s, "pwd"))
        return (3);
    if (ft_strequ(s, "export"))
        return (4);
    if (ft_strequ(s, "unset"))
        return (5);
    if (ft_strequ(s, "env"))
        return (6);
    if (ft_strequ(s, "exit"))
        return (7);
    return (0);
}

int treat_cmd(char **argv, int cmd_id)
{
    if (cmd_id == 1)
        ft_echo(argv);
    else if (cmd_id == 2)
        return(ft_cd(argv));
    else if (cmd_id == 3)
        ft_pwd(argv);
    else if (cmd_id == 4)
        return(ft_export(argv));
    else if (cmd_id == 5)
        ft_unset(argv);
    else if (cmd_id == 6)
        ft_env(argv);
    else if (cmd_id == 7)
        return(ft_exit(argv));
    return (0);
}

int ft_try_path(char **argv)
{
    int i;
    struct stat sb;
    char **env_args = ft_lst_to_array(g_env.env_head);
    char *s = NULL;
    char **pfree;
    char     *tmp;

    i = 0;
    pfree = g_env.path;
    g_env.path = ft_split(get_path(), ':');
    free(pfree);
    // printf("%s", argv[0]);
    while (g_env.path[i])
    {
        s = ft_strjoin(g_env.path[i], "/");
        tmp = s;
        s = ft_strjoin(s, argv[0]);
        free(tmp);
        // printf("%s\n", s);
        if (stat(s, &sb) == 0 && sb.st_mode & S_IXUSR)
		{
            execve(s, argv, env_args);
            free(s);
            return (0);
        }
        free(s);
        i++;
    }
    return (1);
}

int ft_redirect(char **argv)
{
    char **env_args = ft_lst_to_array(g_env.env_head);
    struct stat sb;

    if (stat(argv[0], &sb) == 0 && sb.st_mode & S_IXUSR)
    {
        execve(argv[0], argv, env_args);
        return (0);
    }
    else
    {
        if (!ft_try_path(argv))
            return (0);
    }
    ft_fprintf(2, "minishell: %s: command not found\n", argv[0]);
    return (1);
}

static int    ft_wait()
{
    int         ret;
    int         pid;

    pid = wait(&ret);
    // char *s = (char *)&ret;
    // ft_fprintf(1, "%d %d %d %d\n", s[0], s[1], s[2], s[3]);
    if (WIFEXITED(ret))
        g_minishell.return_code = WEXITSTATUS(ret);
    // ft_close_pipe(cmd);
    g_minishell.forked = 0;
    return (pid);
}


void    execute_command(t_command *cmd)
{
    // t_command   *cmd;
    char        **argv;
    // char        *path;

    
    // cmd = (t_command *) c;
    argv = ft_lst_to_array(cmd->argv);
    // path = ft_strjoin("/bin/", argv[0]);
    // char *env_args[] = { (char*)0 };
    // printf("|%s %d %d|\n", cmd->argv->content, cmd->inRed, cmd->outRed);
    open_redirect_files(cmd);
    dup2(cmd->inRed, 0);
    dup2(cmd->outRed, 1);
    // execve(argv[0], argv, env_args);
    ft_redirect(argv);
    // ft_putstr_fd("minishell: command not found: ", 2);
    // ft_putendl_fd(argv[0], 2);
    exit(127);
}

void    execute_commands()
{
    t_list  *lst;
    t_command   *cmd;
    int         ret;
    char        **argv;
    int         n = 0;

    lst = g_minishell.cmd_head;
    while (lst != NULL)
	{
        cmd = (t_command *)lst->content;
        if (!(cmd = (t_command *)lst->content) || !cmd->argv)
        {
            lst = lst->next;
            continue;
        }
        argv = ft_lst_to_array(cmd->argv);
        // ft_fprintf(2, "CMD: %s %s\n", argv[0], argv[1]);
        // if (cmd->inRed == 0)
        // {
        //     while (n--)
        //         ft_wait();
        //     n = 0;
        // }
        if ((ret = is_command(argv[0])))
        {
            open_redirect_files(cmd);
            // close(cmd->pipe);
            int in;
            int out;
            in = dup(0);
            out = dup(1);
            dup2(cmd->inRed, 0);
            dup2(cmd->outRed, 1);
            g_minishell.return_code = treat_cmd(argv, ret);
            dup2(in, 0);
            dup2(out, 1);
            if (cmd->inRed != 0)
                close(cmd->inRed);
            if (cmd->outRed != 1)
                close(cmd->outRed);

            free(argv);
        } else {
            free(argv);
            if ((ret = fork()) == 0) {
                if (cmd->pipe[0] != -1)
                    close(cmd->pipe[0]);
                if (cmd->pipe[1] != -1)
                    close(cmd->pipe[1]);

                signal(SIGINT, SIG_DFL);
                // print_command(cmd);
                execute_command(lst->content);
            }
            else if (ret < 0)
            {
                ft_fprintf(2, "minishell: fork: %s\n", strerror(errno));
                exit(128);
            }
            else
            {
                if (cmd->inRed != 0)
                    close(cmd->inRed);
                if (cmd->outRed != 1)
                    close(cmd->outRed);

                g_minishell.forked = 1;
                n++;
            }
        }
        lst = lst->next;
    }
    while (n--)
        ft_wait();

    free_redirect_files();
}
