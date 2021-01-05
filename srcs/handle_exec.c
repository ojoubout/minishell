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

#include "minishell.h"


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
    if (!strcmp(s, "echo"))
        return (1);
    if (!strcmp(s, "cd"))
        return (2);
    if (!strcmp(s, "pwd"))
        return (3);
    if (!strcmp(s, "export"))
        return (4);
    if (!strcmp(s, "unset"))
        return (5);
    if (!strcmp(s, "env"))
        return (6);
    if (!strcmp(s, "exit"))
        return (7);
    return (0);
}



void treat_cmd(char **argv, int cmd_id)
{
    if (cmd_id == 1)
        ft_echo(argv);
    else if (cmd_id == 2)
        ft_cd(argv);
    else if (cmd_id == 3)
        ft_pwd(argv);
    else if (cmd_id == 4)
        ft_export(argv);
    else if (cmd_id == 5)
        ft_unset(argv);
    else if (cmd_id == 6)
        ft_env(argv);
    else if (cmd_id == 7)
        ft_exit(argv);
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
            return (1);
        }
        free(s);
        i++;
    }
    return (0);
}

void ft_redirect(char **argv)
{
    int cmd_id;
    char **env_args = ft_lst_to_array(g_env.env_head);
    struct stat sb;
    if ((cmd_id = is_command(argv[0])))
    {
        treat_cmd(argv, cmd_id);
        return ;
    }
    if (stat(argv[0], &sb) == 0 && sb.st_mode & S_IXUSR)
    {
        execve(argv[0], argv, env_args);
        return ;
    }
    else
    {
        if (ft_try_path(argv))
            return ;
    }
    ft_fprintf(2, "minishell: %s: command not found\n", argv[0]);
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

void    ft_close_pipe(t_command *cmd)
{
    if (cmd->inRed != 0)
    {
        close(cmd->inRed);
        ft_fprintf(2, "CLOSE INPUT");
    }
    if (cmd->outRed != 1)
    {
        close(cmd->outRed);
        ft_fprintf(2, "CLOSE OUTPUT");
    }
}

int    ft_wait()
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

int     *ft_new_fd(int in, int out, int pid)
{
    int *fds;

    fds = malloc(3 * sizeof(int));
    fds[0] = in;
    fds[1] = out;
    // fds[2] = pipe;
    fds[2] = pid;
    return (fds);
}

void    ft_close_fds(int pid)
{
    t_list *lst;
    int     *fds;

    lst = g_fds;
    while (lst)
    {
        fds = lst->content;
        if (fds[2] == pid)
        {
            if (fds[0] != 0)
            {
                close(fds[0]);
                // ft_fprintf(2, "FREE IN %d\n", fds[0]);
            }
            if (fds[1] != 1)
            {
                close(fds[1]);
                // ft_fprintf(2, "FREE OUT %d\n", fds[1]);
            }
            break;
        }
        lst = lst->next;
    }
    // if (fds[2] != 0 && fds[2] != 1)
    // {
    //     close(fds[2]);
    //     ft_fprintf(2, "FREE PIPE %d\n", fds[2]);
    // }
}

void ft_close_pipes()
{
    t_list *lst;
    int     *pipes;

    lst = g_pipes;
    while (lst)
    {
        pipes = lst->content;
        close(pipes[0]);
        close(pipes[1]);
        // ft_fprintf(2, "CLOSE %d %d\n", pipes[0], pipes[1]);
        lst = lst->next;
    }
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
            break;
        argv = ft_lst_to_array(cmd->argv);
        
        if ((ret = is_command(argv[0])))
        {
            // open_redirect_files(cmd);
            // close(cmd->pipe);
            int in;
            int out;
            in = dup(0);
            out = dup(1);
            dup2(cmd->inRed, 0);
            dup2(cmd->outRed, 1);
            treat_cmd(argv, ret);
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
                {
                    close(cmd->pipe[0]);
                    // ft_fprintf(2, "CLOSE INPUT PIPE %d\n", cmd->pipe[0]);
                }
                if (cmd->pipe[1] != -1)
                {
                    close(cmd->pipe[1]);
                    // ft_fprintf(2, "CLOSE OUTPUT PIPE %d\n", cmd->pipe[1]);
                }
                // ft_fprintf(2, "CMD: %d %s %d %d\n", ret, cmd->argv->content, cmd->inRed, cmd->outRed);
                // ft_fprintf(2, "FREE D %d\n", cmd->pipe);
                // ft_putendl_fd("FORK", 1);
                signal(SIGINT, SIG_DFL);
                // print_command(cmd);
                execute_command(lst->content);
            }
            else
            {
                if (cmd->inRed != 0)
                {
                    close(cmd->inRed);
                    // ft_fprintf(2, "CLOSE MAIN INPUT PIPE %d\n", cmd->inRed);
                }
                if (cmd->outRed != 1)
                {
                    close(cmd->outRed);
                    // ft_fprintf(2, "CLOSE MAIN OUTPUT PIPE %d\n", cmd->outRed);

                }

                // ft_fprintf(2, "INPUT \n");
                g_minishell.forked = 1;
                n++;
                // printf("CMD: %d %s %d %d\n", ret, cmd->argv->content, cmd->inRed, cmd->outRed);
                // ft_fprintf(2, "CMD: %d %s %d %d\n", ret, cmd->argv->content, cmd->inRed, cmd->outRed);
                // ft_lstadd_back(&g_fds, ft_lstnew(ft_new_fd(cmd->inRed, cmd->outRed, ret)));
                // if (cmd->pipe) {
                //     ft_wait(cmd);
                // }
            }
        }
        lst = lst->next;
        // if (!lst)
            // ft_wait(cmd);
    }
    // lst = g_fds;
    // ft_fprintf(2, "FINISH 1 \n");
    // ft_close_pipes();
    while (n--)
    {
        // ft_fprintf(2, "WAIT \n");
        ft_wait();
        // ft_close_fds(pid);
        // ft_fprintf(2, "FWAIT %d \n", pid);
        // lst = lst->next;
    }
    // ft_fprintf(2, "FINISH 2 \n");
    // write(2, "ERR\n", 4);
    free_redirect_files();

    // ft_fprintf(2, "FINISH 3 \n");

    // ft_lstiter(g_minishell.cmd_head, execute_command);
}
