/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nagaudey <nagaudey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 20:47:43 by nagaudey          #+#    #+#             */
/*   Updated: 2025/07/29 15:47:55 by nagaudey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	**struct_to_array(t_args *args)
{
	char	**array;
	int		size;
	int		i;

	if (!args)
		return (NULL);
	size = find_size_args(args);
	array = malloc(sizeof(char *) * (size + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (args && i < size)
	{
		array[i] = ft_strdup(args->cmd_args);
		if (!array[i])
		{
			free_split(array);
			return (NULL);
		}
		args = args->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

void	exec_dir(t_exec *exec, int dir_result, char ***args_array, char **envp)
{
	if (dir_result == 1)
	{
		exec->cmd_list->cmd_path = ft_strdup(exec->cmd_list->args->cmd_args);
		if (!exec->cmd_list->cmd_path)
			free_child(exec, 1, "malloc", strerror(errno));
		*args_array = struct_to_array(exec->cmd_list->args);
		if (!args_array)
			exit(1);
		if (execve(exec->cmd_list->cmd_path, *args_array, envp) == -1)
		{
			free_split(*args_array);
			if (errno == EISDIR)
				free_child(exec, 126, exec->cmd_list->cmd_path,
					"Is a directory");
			else if (errno == EACCES)
				free_child(exec, 126, exec->cmd_list->cmd_path,
					"Permission denied");
			else
				free_child(exec, 127, exec->cmd_list->cmd_path,
					"No such file or directory");
		}
	}
}

void	ft_execve(t_exec *exec, char ***args_array, char **envp)
{
	if (execve(exec->cmd_list->cmd_path, *args_array, envp) == -1)
	{
		free_split(*args_array);
		if (errno == EISDIR)
			free_child(exec, 126, exec->cmd_list->cmd_path, "Is a directory");
		else if (errno == EACCES)
			free_child(exec, 126, exec->cmd_list->cmd_path,
				"Permission denied");
		else
			free_child(exec, 127, exec->cmd_list->cmd_path,
				"No such file or directory");
	}
}

void	ft_execute(t_exec *exec, char **envp)
{
	char	**args_array;
	int		exit_status;
	int		dir_result;

	if (exec->cmd_list->fd_input == -2 || exec->cmd_list->fd_output == -2)
		exit(1);
	dir_result = is_directory(exec);
	if (dir_result == 126 || dir_result == 127)
		exit(dir_result);
	exec_dir(exec, dir_result, &args_array, envp);
	if (exec->cmd_list->is_builtin)
	{
		exit_status = execute_builtin_in_child(exec, envp);
		free_child(exec, exit_status, NULL, NULL);
	}
	find_path(exec, exec->cmd_list->args->cmd_args);
	if (!exec->cmd_list->cmd_path)
		free_child(exec, 127, exec->cmd_list->args->cmd_args,
			"command not found");
	args_array = struct_to_array(exec->cmd_list->args);
	if (!args_array)
		exit(1);
	ft_execve(exec, &args_array, envp);
}
