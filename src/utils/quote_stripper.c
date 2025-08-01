/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_stripper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almeekel <almeekel@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:12 by almeekel          #+#    #+#             */
/*   Updated: 2025/07/29 17:41:02 by almeekel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "utils.h"

static int	handle_variable_expansion(t_str_builder *sb, const char **ip,
		char **envp)
{
	if (should_expand_variable(**ip, *(*ip + 1), Q_DOUBLE)
		|| should_expand_variable(**ip, *(*ip + 1), Q_NONE))
	{
		if (!process_expansion(sb, ip, envp, 0))
			return (0);
		return (1);
	}
	return (0);
}

static int	process_heredoc_char(t_str_builder *sb, const char **ip,
		char **envp, char *current_quote)
{
	if (!*current_quote && (**ip == '\'' || **ip == '"'))
		return (handle_quote_start(sb, ip, current_quote));
	else if (*current_quote && **ip == *current_quote)
		return (handle_quote_end(sb, ip, current_quote));
	else if (*current_quote == '\'')
		return (handle_single_quote(sb, ip));
	else if (*current_quote == '"' && **ip == '\\' && (*(*ip + 1) == '$'
			|| *(*ip + 1) == '"' || *(*ip + 1) == '\\' || *(*ip + 1) == '\n'))
		return (handle_double_quote_escape(sb, ip));
	else if (handle_variable_expansion(sb, ip, envp))
		return (1);
	else
	{
		if (!sb_append_char(sb, **ip))
			return (0);
		(*ip)++;
	}
	return (1);
}

char	*expand_heredoc_line(const char *line, char **envp)
{
	t_str_builder	sb;
	const char		*ip;
	char			current_quote;

	if (!line)
		return (NULL);
	sb_init(&sb);
	ip = line;
	current_quote = 0;
	while (*ip)
	{
		if (!process_heredoc_char(&sb, &ip, envp, &current_quote))
			return (sb_free_and_return_null(&sb));
	}
	return (sb_to_string_and_free(&sb));
}
