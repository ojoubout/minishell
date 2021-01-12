/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojoubout <ojoubout@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/12 12:10:26 by ojoubout          #+#    #+#             */
/*   Updated: 2021/01/12 12:12:03 by ojoubout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_check_perm(char **env_args, char **argv, struct stat sb, int ret)
{
	if (ret == 0 && sb.st_mode & S_IXUSR && sb.st_mode & S_IRUSR &&
	!S_ISDIR(sb.st_mode) && ft_strchr(argv[0], '/'))
	{
		execve(argv[0], argv, env_args);
		exit(0);
	}
	else if (ret == 0 && ((!(sb.st_mode & S_IXUSR) || !(sb.st_mode & S_IRUSR))
	|| S_ISDIR(sb.st_mode)) && ft_strchr(argv[0], '/'))
	{
		if (S_ISDIR(sb.st_mode))
			ft_fprintf(2, "minishell: %s: is a directory\n", argv[0]);
		else if (!(sb.st_mode & S_IXUSR) || !(sb.st_mode & S_IRUSR))
			ft_fprintf(2, "minishell: %s: Permission denied\n", argv[0]);
		exit(126);
	}
}
