/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeekel <almeekel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:57:50 by nagaudey          #+#    #+#             */
/*   Updated: 2025/07/29 18:48:53 by almeekel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_directory(t_exec *exec)
{
	struct stat	st;
	char		*cmd_path;

	if (!exec || !exec->cmd_list || !exec->cmd_list->args
		|| !exec->cmd_list->args->cmd_args)
		return (0);
	cmd_path = exec->cmd_list->args->cmd_args;
	if (ft_strchr(cmd_path, '/'))
	{
		if (access(cmd_path, F_OK) == -1)
		{
			return (ft_return_message(NULL, cmd_path,
					"No such file or directory", 127));
		}
		if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		{
			return (ft_return_message(NULL, cmd_path, "Is a directory", 126));
		}
		if (access(cmd_path, X_OK) == -1)
		{
			return (ft_return_message(NULL, cmd_path, "Permission denied",
					126));
		}
	}
	return (0);
}

int	execute_builtin(t_exec *exec, char ***envp_ptr)
{
	if (!exec || !exec->cmd_list->args || !exec->cmd_list->args->cmd_args)
		return (0);
	if (ft_strcmp(exec->cmd_list->args->cmd_args, "echo") == 0)
		return (builtin_echo(exec->cmd_list->args));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "cd") == 0)
		return (builtin_cd(exec->cmd_list->args, envp_ptr));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "pwd") == 0)
		return (builtin_pwd(exec->cmd_list->args));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "export") == 0)
		return (builtin_export(exec->cmd_list->args, envp_ptr));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "unset") == 0)
		return (builtin_unset(exec->cmd_list->args, envp_ptr));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "env") == 0)
		return (builtin_env(exec->cmd_list->args, envp_ptr));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "exit") == 0)
		return (builtin_exit(exec, 1));
	else
		return (0);
}

int	is_builtin(t_args *cmd)
{
	if (!cmd || !cmd->cmd_args)
		return (0);
	if (ft_strcmp(cmd->cmd_args, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "export") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "env") == 0)
		return (1);
	if (ft_strcmp(cmd->cmd_args, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin_in_child(t_exec *exec, char **envp)
{
	if (!exec || !exec->cmd_list->args || !exec->cmd_list->args->cmd_args)
		return (0);
	if (ft_strcmp(exec->cmd_list->args->cmd_args, "echo") == 0)
		return (builtin_echo(exec->cmd_list->args));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "pwd") == 0)
		return (builtin_pwd(exec->cmd_list->args));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "env") == 0)
		return (builtin_env(exec->cmd_list->args, &envp));
	else if (ft_strcmp(exec->cmd_list->args->cmd_args, "exit") == 0)
		return (builtin_exit(exec, 0));
	else
		return (0);
}
