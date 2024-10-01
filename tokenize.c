/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tursescu <tursescu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:49:05 by tursescu          #+#    #+#             */
/*   Updated: 2024/10/01 16:01:22 by tursescu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int add_operator(t_token **list, char *line, int i)
{
	t_token *new;
    int     op_len;
    char    *temp;
	
    op_len = is_operator(&line[i]);
	if (op_len > 0)
    {
        temp = ft_strndup(&line[i], op_len);         //proper alloc so we don't have overwritning
        new = create_token(set_type(&line[i]), temp);
        free(temp);
		append_token(list, new);
		i += op_len;
    }
	return (i);
}
int	add_quote(t_token **list, char *line, int i)
{
	t_token	*new;
	char	quote;
	int		start;
	char	*temp;

	quote = line[i];
	start = i++;//skip the beginning quote
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
		i++;//skip the end quote
	temp = ft_strndup(&line[start], i - start);
	new = create_token(set_type(&line[start]), temp);
	free(temp);
	append_token(list, new);
	return (i);
}

int	add_words(t_token **list, char *line, int i)
{
	t_token	*new;
	int		start;
	char	*sub;
	
	start = i;
	while (line[i] && is_word(&line[i]))
		i++;
	if (start != i)
	{
		sub = ft_substr(line, start, i - start);
		new = create_token(T_WORD, sub);
		free(sub);
		append_token(list, new);
	}
	return (i);
}

t_token	*tokenize(char *line)
{
	t_token	*tokens;
	int		i;
	
	i = 0;
	tokens = NULL;
	while (line[i])
	{
		i = skip_whitespace(line, i);
		if (is_operator(&line[i]))
			i = add_operator(&tokens, line, i);
		else if (line[i] == '\'' || line[i] == '"')
			i = add_quote(&tokens, line, i);
		else
			i = add_words(&tokens, line, i);
	}
	return(tokens);
}