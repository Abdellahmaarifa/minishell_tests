/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaarifa <amaarifa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 01:34:52 by amaarifa          #+#    #+#             */
/*   Updated: 2022/05/21 12:09:38 by amaarifa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
char	*expand_var(char *value)
{
	int	i;
	int	start;
	int	end;
	char	*s[5];
	int expand;


	i = 0;
	while (i < 5)
		s[i++] = NULL;
	i = 0;
	expand = 0;
	while (value[i])
	{
		if (value[i] == '\"')
			expand = !expand;
		if (value[i] == '\'' && !expand)
		{
			i++;
			while (value[i] && value[i] != '\'')
				i++;
			i++;
			continue ; 
		}
		if (value[i] == '$')
		{
			s[0] = ft_substr(value, 0, i);
			start = ++i;
			while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
				i++;
			end = i;
			s[2] = ft_substr(value, i, ft_strlen(value) - i);
			s[1] = ft_substr(value, start, i - start);
			s[3] = getenv(s[1]);
			if (s[3])
			{
				free(value);
				s[4] = ft_strjoin(s[0], s[3]);
				value = ft_strjoin(s[4], s[2]);
				i = ft_strlen(s[0]) + ft_strlen(s[1]);
				continue ;
			}
			else
			{
				free(value);
				value = ft_strjoin(ft_strjoin(s[0], s[2]), value + end);
				if (value[end])
				{
					i = end;
					continue ;
				}
			}
		}
		
		i++;
	}
	 i = 0;
	 while (i < 4)
	 {
		if (i != 3 && s[i])
			free(s[i++]);
		i++;
	 }

	return value;
}

*/

char	*join_strings(char *s1, char *s2)
{
	char *s;
	s = ft_strjoin(s1, s2);
	free(s1);
	//free(s2);
	return (s);
}
char *get_env(char *s)
{
	char *str;

	str = getenv(s);
	if (!str)
		return (ft_strdup(""));
	return (str);
}

int unclosed_double_qoutes(char *s, int i)
{
	int		j;
	int		n;

	n = 0;
	j = 0;
	while (s[j] && j <= i)
	{
		if (s[j] == '\"')
			n = !n;
		j++;
	}
	return (n);
}

int unclosed_single_qoutes(char *s, int i)
{
	int		j;
	int		n;

	n = 0;
	j = 0;
	while (s[j] && j <= i)
	{
		if (s[j] == '\'')
			n = !n;
		j++;
	}
	return (n);
}

char	*expand_var(char *value)
{
	int		i;
	char	*s;
	int		start;
	char	*temp;
	int		expand;

	expand = 0;
	s = ft_strdup("");
	i = 0;
	while (value[i])
	{
		if (value[i] != '$')
		{
			start = i;
			while (value[i] && value[i] != '$')
			{
				if (value[i] == '\'' && !unclosed_double_qoutes(value, i - 1))
				{	
					i++;
					while (value[i] && value[i] != '\'')
						i++;
					
				}
				i++;
			}
			if (start == i)
				temp = ft_substr(value, start, i - start + 1);
			else
				temp = ft_substr(value, start, i - start);
			s = join_strings(s, temp);
			free(temp);
			continue ;
		}
		else {
			i++;
			while (value[i] && value[i] == '$')
				i++;
			if (!value[i])
				break ;
			start = i;
			while (value[i] && (ft_isalnum(value[i]) || value[i] == '_'))
				i++;
			if (start == i)
				temp = ft_substr(value, start, i - start + 1);
			else
				temp = ft_substr(value, start, i - start);
			s = join_strings(s, get_env(temp));
			free(temp);
			continue ;
		}
		i++;
	}
	free(value);
	return (s);
}




t_token *word_spliter(t_token *token)
{
	
	return create_token("hello", 0);
}

void add_last_token(t_token *token, t_token *new_token)
{
	t_token *temp;

	temp = token;
	while (temp)
	{
		if (!temp->next)
			break;
		temp = temp->next;
	}
	temp = new_token;
}

int list_size (t_token *token)
{
	int i;

	i = 0;
	while (token)
	{
		i++;
		token = token->next;
	}
	return (i);
}

void print_test(t_token *node)
{
	t_token *temp;

	temp = node;
	while(temp)
	{
		printf("temp value == %s\n", temp->value);
		temp = temp->next;
	}
}

void	replace_space(char	*s)
{
	int	i;

	i = 0;
	while (s[i])
	{

		if (s[i] == ' ' && !unclosed_double_qoutes(s, i)
			&& !unclosed_single_qoutes(s, i))
			s[i] = 8;
		i++;
	}
}

t_token	*parse_token_value(char *value)
{
	char	**s;
	int		i;
	t_token	*tokens;
	t_token	*temp;

	
	replace_space(value);
	s = ft_split(value, 8);
	tokens = create_token(s[0], ARG);
	i = 1;
	temp = tokens;
	while (s[i])
	{
		tokens->next = create_token(s[i], ARG);
		tokens = tokens->next;
		i++;
	}
	free(s);
	free(value);
	return (temp);
}


t_token	*insert_tokens(t_token *prev, char *value, t_token *next)
{
	t_token *new_token;

	new_token = parse_token_value(value);
	if (prev)
		prev->next = new_token;
	add_last_token(new_token, next);
	//free(*token);
	return new_token;
}

int	check_expand(t_token *token)
{
	int	i;

	i = 0;
	while ((token->value)[i])
	{
		if ((token->value)[i] == '\'')
			return (0);
		else if ((token->value)[i] == '\"')
			return (1);
		i++;
	}
	return (1);
}


int get_index(char *s, char c)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	check_word_split(t_token *token)
{
	int i;
	int j;

	i = get_index(token->value, '$');
	if (i == -1)
		return (0);
	else
	{
		if (unclosed_double_qoutes(token->value, i - 1)
		|| unclosed_single_qoutes(token->value, i- 1))
			return (0);
	}
	return (1);
}

t_token	*get_last_token(t_token *token)
{
	t_token *temp;

	temp = token;
	while (temp)
	{
		if (!temp->next)
			break;
		temp = temp->next;
	}
	return  (temp);
}

void	expander(t_token **token)
{
	t_token	*temp;
	t_token	*next;
	t_token	*prev;
	t_token	*temp_token;

	temp = *token;
	while (temp)
	{
		//temp->word_split = check_word_split(temp);
		temp->value =  expand_var(temp->value);
		temp = temp->next;
	}
		
	temp = *token;
	prev = NULL;
	t_token *freed_tk;
	t_token *freed_tmp;
	freed_tk = create_token("null", 0);
	freed_tmp = freed_tk;
	
	int		p = 0;
	// while (temp)
	// {	
	// 	next = temp->next;
	// 	if (ft_strchr(temp->value, ' '))
	// 	{
	// 		temp = insert_tokens(prev, temp->value, next);
	// 		if (!prev)
	// 		{
	// 			// if ((*token))
	// 			// 	free((*token)->value);
	// 			free(*token);
	// 			*token = temp;
	// 			// break ;
	// 		}
	// 		prev = get_last_token(temp);
	// 		temp = next;
	// 		continue ;
	// 	} else 
	// 		prev = temp;
	// 	temp = next;
	// }
	//if (freed_tk)
	//	free(freed_tk);
	//while(1);
	//print_token(token);
	//printf("end test\n\n\n\n");
}
