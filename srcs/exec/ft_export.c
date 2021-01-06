/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmoumani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/01 14:13:47 by hmoumani          #+#    #+#             */
/*   Updated: 2021/01/01 14:13:49 by hmoumani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_valid_identifier(char *s)
{
    int i;

    i = 0;
    if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
        return (0);
    while (s[i])
    {
        if (!ft_isalpha(s[i]) && s[i] != '_' && !ft_isdigit(s[i]))
			if (!(s[i] == '+' && !s[i + 1]))
            	return (0);
        i++;
    }
    return (1);
}

int custem_len(char *s)
{
    int i;

    i = 0;
    while (s[i] && s[i] != '=')
	{
		if (s[i] == '+' && s[i + 1] == '=')
			return (i);
        i++;
	}
    return (i);
}

t_list *lstchr(t_list *head, char *s)
{
    t_list *curr;
    int len;

    if (!s)
        return (NULL);
    len = custem_len(s);
    curr = head;
    while (curr)
    {
        if (!ft_strncmp(s, curr->content, len) && (*(((char *)curr->content) + len) == '=' ||
        *(((char *)curr->content) + len) == '\0'))
            return (curr);
        curr = curr->next;
    }
    return (NULL);
}

void ft_print_path(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        ft_putchar_fd(s[i], 1);
        if (s[i] == '=')
            break;
        i++;
    }
    if (!strchr(s, '='))
        return ;
    i++;
    ft_putchar_fd('"', 1);
    while (s[i])
    {
        ft_putchar_fd(s[i], 1);
        i++;
    }
    ft_putchar_fd('"', 1);
}

void show_all_export(char *start_with)
{
    t_list *curr;

    curr = g_env.env_head;
	start_with = NULL;
    while (curr)
    {
		// ft_fprintf(2, "%s\n", curr->content);
        if (!strncmp(curr->content, "_=", 2))
        {
            curr = curr->next;
            continue ;
        }
        ft_putstr_fd(start_with, 1);
        ft_print_path(curr->content);
        ft_putstr_fd("\n", 1);
        curr = curr->next;
    }
}

int    ft_export(char **argv)
{
    if (!argv[1])
        show_all_export("declare -x ");
    else
        return(export_all(argv));
    return (0);
}

int ft_ptr_str_len(char **ptr)
{
    int i;

    i = 0;
    while (ptr[i])
        i++;
    return (i);
}

void export_name(char *argv)
{
    if (lstchr(g_env.env_head, argv))
        return ;
    ft_lstadd_back(&g_env.env_head, ft_lstnew(ft_strdup(argv)));
}

void export_empty_string(char **argv, char **sp, int i)
{
    t_list *node;
    char *pfree;

    if ((node = lstchr(g_env.env_head, argv[i])))
    {
        pfree = node->content;
        node->content = ft_strjoin(argv[i], "");
        free(pfree);
    }
    else
    {
        ft_lstadd_back(&g_env.env_head, ft_lstnew(ft_strjoin(argv[i], "")));
    }
    sp = NULL;
}

void export_normal(char *string)
{
    t_list *node;
    char *pfree;

    if ((node = lstchr(g_env.env_head, string)))
    {
        pfree = node->content;
        node->content = ft_strdup(string);
        free(pfree);
    }
    else
    {
        ft_lstadd_back(&g_env.env_head, ft_lstnew(ft_strdup(string)));
    }
}


void add_element(char *key, char *value)
{
    char *pfree;
    char *holder;

    holder = ft_strjoin(key, "=");
    pfree = holder;
	if (value)
	{
    	holder = ft_strjoin(holder, value);
    	free(pfree);
	}
    export_normal(holder);
}

void export_append(char *string, char **sp, int len)
{
	*ft_strchr(sp[0], '+') = 0;
	ft_fprintf(2, "%s\n", sp[1]);
    t_list *node;
	char **tmp;
    // char *pfree;

	tmp = NULL;
    if ((node = lstchr(g_env.env_head, string))) // variable exist
    {
		if (len == 1)							// export ll+=
			export_name(sp[0]);
		else									// export ll+=test
		{
			tmp = ft_split(node->content, '=');
			if (ft_ptr_str_len(tmp) == 1)		// variable exist with empty value ex: ll
				add_element(sp[0], sp[1]);
			else if (ft_ptr_str_len(tmp) == 2)	// variable exist with  value ex: ll=test
				add_element(sp[0], ft_strjoin(tmp[1], sp[1]));
		}
    }
    else
    {
        add_element(sp[0], sp[1]);
    }
}

int  export_all(char **argv)
{
    char **sp;
    int i;
    int ret;

    ret = 0;
    i = 1;
    while (argv[i])
    {
        sp = ft_split(argv[i], '=');
        // ft_fprintf(2, "%p ||| %p\n", argv[i], ft_strchr(argv[i], '='));
        // ft_fprintf(2, "%p\n", sp[0]);
        if (!is_valid_identifier(sp[0]) || argv[i][0] == '=')
        {
            g_minishell.return_code = 1;
            ft_fprintf(2, "minishell: export: `%s': not a valid identifier\n", argv[i]);
            i++;
            ret = 1;
            continue ;
        }
        if (ft_ptr_str_len(sp) == 1 && !ft_strchr(argv[i], '='))				//export ll
            export_name(argv[i]);
		else if (sp[0][ft_strlen(sp[0]) - 1] == '+' && ft_strchr(argv[i], '='))	// export +=...
			export_append(argv[i], sp, ft_ptr_str_len(sp));
        else if (ft_ptr_str_len(sp) == 1 && ft_strchr(argv[i], '=')) 			// export ll=
            export_empty_string(argv, sp, i);
        else                                                           			//export ll=normal
            export_normal(argv[i]);
        ft_free_split(sp);
        i++;
    }
    return (ret);
}