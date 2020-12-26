/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/13 12:58:08 by ojoubout          #+#    #+#             */
/*   Updated: 2020/12/13 12:58:11 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_error(char *str)
{
    ft_putendl_fd(str, 2);
    exit(1);
}

t_command   *ft_new_command(int in, int out)
{
    t_command   *cmd;

    cmd = malloc(sizeof(t_command));
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->inRed = in;
    cmd->outRed = out;
    return (cmd);
}

void    show_prompt()
{    
    if (g_minishell.return_code)
        write(1, BBLU "minishell "BRED"> "RESET, 31);
    else
        write(1, BBLU "minishell "BGRN"> "RESET, 31);
}

void    ft_split_free(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void    ft_free_command(void *cmd)
{
    ft_lstclear(&((t_command *) cmd)->argv, free);
    free(cmd);
    // return NULL;
}

int ft_skip_space(const char *str)
{
    int i;

    i = 0;
    while (str[i] == ' ')
        i++;
    return (i);
}

int get_next_word(const char *str, char *d)
{
    int     i;
    char    quote[2];

    i = ft_skip_space(str);
    ft_bzero(quote, 2);

    while (str[i])
    {
        // write(1, str + i, 1);
        if (!quote[0] || ft_on_char(str, i, quote))
		{
			if (ft_on_char(str, i, "'\""))
                quote[0] = quote[0] ? 0 : str[i];
            if (!ft_on_char(str, i, d) || ft_on_char(str, i, d))
				break;
        }
        i++;
    }
    i += ft_skip_space(str + i);

    return (i);
}

int    ft_precess_cmd(char *str)
{
    t_command   *cmd;

    cmd = g_minishell.cmd_tail->content;
    if (((ft_strncmp(str, OUTPUT_RED, 1) == 0 || ft_strncmp(str, APP_OUTPUT_RED, 2) == 0 
        || ft_strncmp(str, INPUT_RED, 1) == 0) && g_minishell.read_next != NULL) ||
        (ft_strncmp(str, PIPE, 1) == 0 && (g_minishell.read_next != NULL || cmd->argv == NULL)))
        return ft_syntax_error(str);
    // else if (*str !=)
    if (ft_strncmp(str, PIPE, 1) == 0)
        ft_handle_pipe(str);
    else if (ft_strncmp(str, INPUT_RED, 1) == 0)
        ft_handle_input_red(str);
    else if (ft_strncmp(str, APP_OUTPUT_RED, 2) == 0)
        ft_handle_output_red(str, APP_OUTPUT_RED);
    else if (ft_strncmp(str, OUTPUT_RED, 1) == 0)
        ft_handle_output_red(str, OUTPUT_RED);
    else
        ft_handle_cmd(str);
    return 0;
}

int     main(void)
{
    signal(SIGINT, handle_sigint);
    g_minishell.return_code = 0;
    while (1)
    {
        g_minishell.cmd_head = ft_lstnew(ft_new_command(0, 1));
        g_minishell.cmd_tail = g_minishell.cmd_head;
        g_minishell.stat = 1;
        g_minishell.forked = 0;
        g_minishell.read_next = NULL;
        show_prompt();
        get_next_line(0, &g_minishell.command_line);
        // split ; then | then < > >>
        // char **cmds = ft_sh_split(g_minishell.command_line, ';');
        // int i = 0;
        int len = ft_strlen(g_minishell.command_line);
        g_minishell.pos = 0;
        // int pos = 0;
        // ft_precess_cmd(g_minishell.command_line);
        while (g_minishell.stat == 1 && g_minishell.command_line[g_minishell.pos] &&
        (g_minishell.pos += get_next_word(g_minishell.command_line + g_minishell.pos, SEP)) != -1)
        {
            // if (ft_on_char(g_minishell.command_line + pos, 0, SEP))
                // pos++;
            len = ft_word_length(g_minishell.command_line + g_minishell.pos, SEP);
            // ft_putnbr_fd(len, 1);
            // ft_putendl_fd(g_minishell.command_line + g_minishell.pos, 1);
            ft_precess_cmd(g_minishell.command_line + g_minishell.pos);
            g_minishell.pos += len;
            // g_minishell.pos++;
        }
        if (g_minishell.read_next != NULL)
            ft_syntax_error("\n");

        // print_commands();
        execute_commands();
        free(g_minishell.command_line);
        ft_lstclear(&g_minishell.cmd_head, ft_free_command);
    }
    return (EXIT_SUCCESS);
}