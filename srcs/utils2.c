/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/11 17:15:07 by ojoubout          #+#    #+#             */
/*   Updated: 2021/01/11 18:03:27 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**ft_lst_to_array(t_list *lst)
{
	char	**argv;
	int		len;
	int		i;

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

t_list		*ft_array_to_lst(char **array)
{
	t_list	*head;
	int		i;

	i = 0;
	head = NULL;
	while (array[i])
	{
		ft_lstadd_back(&head, ft_lstnew(ft_strdup(array[i])));
		i++;
	}
	return (head);
}

void		ft_lstadd(t_list **lst, t_list *new)
{
	if (*lst == NULL || new == NULL)
		return ;
	new->next = (*lst)->next;
	(*lst)->next = new;
}

void		ft_lstremove(t_list **lst, t_list *del_lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!(*lst) || !del_lst)
		return ;
	if (*lst == del_lst)
	{
		*lst = (*lst)->next;
		if (del)
			ft_lstdelone(del_lst, del);
		return ;
	}
	tmp = *lst;
	while (tmp->next)
	{
		if (tmp->next == del_lst)
		{
			tmp->next = tmp->next->next;
			if (del)
				ft_lstdelone(del_lst, del);
		}
		tmp = tmp->next;
	}
}