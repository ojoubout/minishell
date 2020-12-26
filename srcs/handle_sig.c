#include "minishell.h"

void    handle_sigint(int sig)
{
    if (sig == SIGINT) {
        ft_putstr_fd("\b\b  \b\b", 1);
        write(1, "\n", 1);
        g_minishell.return_code = 1;
        if (!g_minishell.forked)
        {
            g_minishell.return_code = 130;
            show_prompt();
        }
    }
}