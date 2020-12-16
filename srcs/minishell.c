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

void    ft_error(const char *str)
{
    ft_putendl_fd(str, 2);
    exit(1);
}

t_command   *ft_new_command(int in, int out)
{
    t_command   *cmd;

    cmd = malloc(sizeof(t_command));
    cmd->argc = 0;
    cmd->inRed = in;
    cmd->outRed = out;
    return (cmd);
}

void    show_prompt()
{    
    if (g_minishell.return_code)
        write(1, BBLU " minishell "BRED"> "RESET, 32);
    else
        write(1, BBLU " minishell "BGRN"> "RESET, 32);
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
            if (ft_on_char(str, i, d))
				break;
        }
        i++;
    }
    i += ft_skip_space(str + i);

    return (i);
}

void    ft_precess_cmd(char *str)
{
    if (ft_strncmp(str, PIPE, 1) == 0)
        ft_handle_pipe(str);
    else if (ft_strncmp(str, INPUT_RED, 1) == 0)
        ft_handle_input_red(str);
    else if (ft_strncmp(str, APP_OUTPUT_RED, 2) == 0)
        ft_handle_output_red(str, 1);
    else if (ft_strncmp(str, OUTPUT_RED, 1) == 0)
        ft_handle_output_red(str, 0);
    else
        ft_handle_cmd(str);
}

int     main(void)
{
    g_minishell.return_code = 0;
    g_minishell.stat = 1;
    g_minishell.cmd_head = ft_lstnew(NULL);
    while (g_minishell.stat)
    {
        show_prompt();
        get_next_line(0, &g_minishell.command_line);
        // split ; then | then < > >>
        // char **cmds = ft_sh_split(g_minishell.command_line, ';');
        // int i = 0;
        int len = ft_strlen(g_minishell.command_line);
        int pos = 0;
        while (g_minishell.command_line[pos] && (pos += get_next_word(g_minishell.command_line + pos, " ;|<>")) != -1)
        {
            // if (ft_on_char(g_minishell.command_line + pos, 0, " ;|<>"))
                // pos++;
            len = ft_word_length(g_minishell.command_line + pos, " ;|<>");
            ft_putnbr_fd(len, 1);
            ft_putendl_fd(g_minishell.command_line + pos, 1);
            pos++;
        }
    }
    return (EXIT_SUCCESS);
}