/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 17:14:39 by ojoubout          #+#    #+#             */
/*   Updated: 2021/01/11 18:04:48 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_wait(void)
{
	int		ret;
	int		pid;

	pid = wait(&ret);
	if (WIFEXITED(ret))
		g_minishell.return_code = WEXITSTATUS(ret);
	g_minishell.forked = 0;
	return (pid);
}

static void	ft_builtin_cmd(t_command *cmd, char **argv, int ret)
{
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
}

static void	ft_system_cmd(t_command *cmd, int ret, int *n)
{
	if ((ret = fork()) == 0)
	{
		if (cmd->pipe[0] != -1)
			close(cmd->pipe[0]);
		if (cmd->pipe[1] != -1)
			close(cmd->pipe[1]);
		signal(SIGINT, SIG_DFL);
		execute_command(cmd);
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
		(*n)++;
	}
}

static int	ft_init_exec(t_command *cmd, int *n, char ***argv)
{
	open_redirect_files(cmd);
	if (!cmd || !cmd->argv || cmd->inRed == -1 || cmd->outRed == -1)
		return (1);
	if (cmd->inRed == 0)
	{
		while ((*n)--)
			ft_wait();
		*n = 0;
	}
	ft_argv_convert_env(&cmd->argv);
	*argv = ft_lst_to_array(cmd->argv);
	return (0);
}

void		execute_commands(void)
{
	t_list		*lst;
	t_command	*cmd;
	int			ret;
	char		**argv;
	int			n;

	lst = g_minishell.cmd_head;
	n = 0;
	while (lst != NULL)
	{
		open_redirect_files(cmd = lst->content);
		if (ft_init_exec(cmd, &n, &argv) && ((lst = lst->next) || 1))
			continue;
		if ((ret = is_command(argv[0])))
			ft_builtin_cmd(cmd, argv, ret);
		else
		{
			free(argv);
			ft_system_cmd(cmd, ret, &n);
		}
		lst = lst->next;
	}
	while (n--)
		ft_wait();
	free_redirect_files();
}
