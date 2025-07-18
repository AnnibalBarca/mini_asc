/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeekel <almeekel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:37:24 by almeekel          #+#    #+#             */
/*   Updated: 2025/07/18 11:12:38 by almeekel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

static int	replace_env_var(char ***env_ptr, int index, char *name, char *value)
{
	char	*new_var_string;
	char	*old_var;

	new_var_string = create_env_string(name, value);
	if (!new_var_string)
		return (1);
	old_var = (*env_ptr)[index];
	(*env_ptr)[index] = new_var_string;
	free(old_var);
	return (0);
}

static int	add_new_env_var(char ***env_ptr, char *name, char *value, int i)
{
	char	**new_env;
	char	*new_var_string;
	int		count;

	new_var_string = create_env_string(name, value);
	if (!new_var_string)
		return (1);
	count = count_env_vars(*env_ptr);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var_string);
		return (1);
	}
	while (i < count)
	{
		new_env[i] = (*env_ptr)[i];
		i++;
	}
	new_env[count] = new_var_string;
	new_env[count + 1] = NULL;
	free(*env_ptr);
	*env_ptr = new_env;
	return (0);
}

char	*find_env_value(char **envp, const char *name)
{
	int		index;
	char	*equals_pos;

	index = find_env_index(envp, name);
	if (index == -1)
		return (NULL);
	equals_pos = ft_strchr(envp[index], '=');
	if (!equals_pos)
		return (NULL);
	return (equals_pos + 1);
}

int	set_env_var(char ***env_ptr, char *name, char *value)
{
	int	var_index;

	if (!env_ptr || !*env_ptr || !name)
		return (1);
	var_index = find_env_index(*env_ptr, name);
	if (var_index != -1)
		return (replace_env_var(env_ptr, var_index, name, value));
	else
		return (add_new_env_var(env_ptr, name, value, 0));
}
