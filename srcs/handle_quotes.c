/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 14:55:21 by ojoubout          #+#    #+#             */
/*   Updated: 2021/01/11 18:05:11 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_is_backslashed(const char *str, int i)
{
	int is_backslashed;

	i--;
	is_backslashed = 0;
	while (i >= 0 && str[i] == '\\')
	{
		is_backslashed = !is_backslashed;
		i--;
	}
	return (is_backslashed);
}

static int	ft_is_quoted(const char *str, int i)
{
	char	quote[2];
	int		j;

	ft_bzero(quote, 2);
	j = 0;
	while (j <= i)
	{
		if (!quote[0] && (str[j] == '"' || str[j] == '\'') &&
		!ft_is_backslashed(str, j))
			quote[0] = str[j];
		else if ((quote[0] == '\'' && str[j] == '\'') ||
		(quote[0] == '"' && str[j] == '\"' && !ft_is_backslashed(str, j)))
			quote[0] = 0;
		j++;
	}
	return (quote[0] == '\'' && str[i] != '\'');
}

int			ft_on_char(const char *str, int i, char *c)
{
	int		j;
	char	*quote[2];

	j = 0;
	ft_bzero(quote, 2);
	while (c[j])
	{
		if (str[i] == c[j])
			if (i == 0 || (!ft_is_backslashed(str, i) && !ft_is_quoted(str, i)))
				return (1);
		j++;
	}
	return (0);
}

char		*ft_quotes_convert(char *str)
{
	int		i;
	char	*res;
	char	*tmp;
	char	quote[2];

	i = 0;
	ft_bzero(quote, 2);
	res = ft_strdup("");
	while (str[i])
	{
		if (quote[0] != '\'' && ft_on_char(str, i, "\\") && ++i)
			continue;
		if ((!quote[0] && ft_on_char(str, i, "'\"")) ||
		(quote[0] == '\'' && str[i] == '\'') || ft_on_char(str, i, quote))
			quote[0] = quote[0] ? 0 : str[i];
		else
		{
			tmp = res;
			res = ft_strappend(res, str[i]);
			free(tmp);
		}
		i++;
	}
	free(str);
	return (res);
}

int			ft_word_length(const char *s, char *c)
{
	int		i;
	char	quote[2];

	i = 0;
	ft_bzero(quote, 2);
	while (s[i])
	{
		if (!quote[0] || ft_on_char(s, i, quote))
		{
			if (ft_on_char(s, i, "'\""))
				quote[0] = quote[0] ? 0 : s[i];
			if (ft_on_char(s, i, c))
				break ;
		}
		i++;
	}
	return (i);
}

void		ft_argv_convert_env(t_list **argv)
{
	t_list	*tmp;
	t_list	*prev;

	if (!(*argv))
		return ;
	tmp = *argv;
	while (tmp)
	{
		tmp->content = ft_convert_env(tmp->content);
		if (*((char *)tmp->content) == 0)
		{
			if (*argv == tmp)
			{
				ft_lstremove(&tmp, tmp, ft_free);
				*argv = tmp;
			}
			else
			{
				ft_lstremove(&tmp, tmp, ft_free);
				prev->next = tmp;
			}
		}
		else
		{
			tmp->content = ft_quotes_convert(tmp->content);
			prev = tmp;   
			tmp = tmp->next;
		}
	}
}